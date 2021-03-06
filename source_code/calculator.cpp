/**
    2D Function plot with built-in math expression interpreter
    Copyright (C) 2009-2011  Leonardo Berti

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    contact: leonardo.berti@tin.it
*/


#include "calculator/calculator.h"
#include <map>
#include <vector>
#include <string>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>


using namespace std;


namespace calculator {

///////////////////// StringInputStream /////////////////////////////

    StringInputStream::StringInputStream(const string& str_prog) {
        program=str_prog;
        cur_pos=0;
        llen=program.size();

    }

    char StringInputStream::GetNextChar() throw (EndOfInput) {
        if (cur_pos>=llen) throw EndOfInput();
        return program[cur_pos++];
    }

    void StringInputStream::PutBack() {

        if (cur_pos>0) --cur_pos;
    }

////////////////////////// ConsoleUserInput ////////////////////////////

    
    double ConsoleUserInput::GetDouble(const string& message) const
    {
        cout<<message<<endl;
        double d;
        while (!(cin>>d))
        {
            cout<<"Bad input!"<<endl;
            cout<<message<<endl;
            cin.clear();
            cin.sync();
        }

        return d;
    }

////////////////////////// Calculator ///////////////////////////////////

    Calculator::Calculator() {

        input_stream=0;
        output_stream=0;
        cur_sym.token=END;
        Init();
    }

    Calculator::~Calculator() {
    }

    void Calculator::Init() {
        output_stream=0;
        last_var=0;
        //built in functions
        sing_var_functions["sin"]=sin;
        sing_var_functions["cos"]=cos;
        sing_var_functions["tan"]=tan;
        sing_var_functions["acos"]=acos;
        sing_var_functions["asin"]=asin;
        sing_var_functions["atan"]=atan;
        sing_var_functions["ceil"]=ceil;
        sing_var_functions["cosh"]=cosh;
        sing_var_functions["exp"]=exp;
        sing_var_functions["fabs"]=fabs;
        sing_var_functions["floor"]=floor;
        sing_var_functions["log"]=log;
        sing_var_functions["ln"]=log;
        sing_var_functions["log10"]=log10;
        sing_var_functions["sinh"]=sinh;
        sing_var_functions["sqrt"]=sqrt;
        sing_var_functions["tanh"]=tanh;
        //built in functions definite in Calculator
        sing_var_functions["p2"]=p2; //x*x
        sing_var_functions["p3"]=p3; //x*x*x
        sing_var_functions["p4"]=p4;
        sing_var_functions["p5"]=p5;
        sing_var_functions["p6"]=p6;
        sing_var_functions["p7"]=p7;
        sing_var_functions["p8"]=p8;
        sing_var_functions["p9"]=p9;
        sing_var_functions["p10"]=p10;
        sing_var_functions["abs"]=Calculator::abs;
        sing_var_functions["sgn"]=Calculator::sgn;

        double_var_functions["pow"]=pow; //x alla y

        //built-in consts
        vars["e"]=M_E;
        vars["pi"]=M_PI; //pi greco
        vars["pi2"]=M_PI_2; //pi greco/2
        vars["pi4"]=M_PI_4;

        //str functions
        str_str_functions["print"]=Calculator::print;
        str_str_functions["println"]=Calculator::println;
        //comandi
        cmd_functions["dump"]=Calculator::dump;

        reserved_words.push_back("input");

        for (map<string,SINGLE_ARG_FUNC>::iterator it=sing_var_functions.begin();it!=sing_var_functions.end();it++) {
            reserved_words.push_back((*it).first);
        }

        for (map<string,DOUBLE_ARG_FUNC>::iterator it=double_var_functions.begin();it!=double_var_functions.end();it++)
        {
            reserved_words.push_back((*it).first);
        }

        for (map<string,double>::iterator it=vars.begin();it!=vars.end();it++) {
            reserved_words.push_back((*it).first);
        }

        for (map<string,STR_FUNC>::iterator it=str_str_functions.begin();it!=str_str_functions.end();it++) {
            reserved_words.push_back((*it).first);
        }

        for (map<string,CMD_FUNC>::iterator it=cmd_functions.begin();it!=cmd_functions.end();it++)
        {
            reserved_words.push_back((*it).first);
        }
    }

    /**controlla se il simbolo � una parola riservata*/
    void Calculator::CheckSymbol(const string& name) throw (ReservedNameException) {
        
        vector<string>::const_iterator it=reserved_words.begin();

        while (it!=reserved_words.end()) {

            if (*it==name) throw ReservedNameException();

            it++;
        }
       
    }

    double Calculator::SetVar(const string& var_name,double value) throw (ReservedNameException) {

        CheckSymbol(var_name);

        last_var=&vars[var_name];
        (*last_var)=value;

        return value;
    }

    double Calculator::SetLastVar(double value)
    {
        (*last_var)=value;
        return value;
    }

    double Calculator::GetVar(const string& var_name) throw (SymbolNotFound) {
        return vars[var_name];
    }

    /**Aggiunge una funzione*/
    void Calculator::SetFunction(const string& f_name,SINGLE_ARG_FUNC f) throw (ReservedNameException) {
        CheckSymbol(f_name);
        sing_var_functions[f_name]=f;
    }

    /**rimuove una funzione*/
    void Calculator::RemoveFunction(const string& f_name) throw (ReservedNameException,SymbolNotFound) {
        CheckSymbol(f_name);
        map<string,SINGLE_ARG_FUNC>::iterator it=sing_var_functions.find(f_name);
        if (it==sing_var_functions.end()) throw SymbolNotFound();
        sing_var_functions.erase(f_name);
    }

    /**Aggiunge una funzione con argomento stringa*/
    void Calculator::SetFunction(const string& f_name,STR_FUNC f) throw (ReservedNameException) {
        CheckSymbol(f_name);
        str_str_functions[f_name]=f;
    }

    void Calculator::RemoveStrFunction(const string& f_name) throw (ReservedNameException,SymbolNotFound) {
        CheckSymbol(f_name);
        map<string,STR_FUNC>::iterator it=str_str_functions.find(f_name);
        if (it==str_str_functions.end()) throw SymbolNotFound();
        str_str_functions.erase(f_name);
    }

    /**Rimuove una variabile*/
    void Calculator::RemoveVar(const string& var_name) throw (SymbolNotFound,ReservedNameException) {
        CheckSymbol(var_name);
        map<string,double>::iterator it=vars.find(var_name);
        if (it==vars.end()) throw SymbolNotFound();
        vars.erase(var_name);
    }

    //static
    /**funzione usata per l'output*/
    string Calculator::print(const string& e,const OutputStream* out) {

        if (!out) return "";
        out->print(e);
        return e;
    }

    string Calculator::println(const string& e,const OutputStream* out)
    {
        if (!out) return "";
        out->println(e);
        return e;
    }

    /**Esegue il dump delle variabili e costanti*/
    void Calculator::dump(const OutputStream* out,Calculator& c)
    {
        if (!out) return;
        const size_t mem=64;
        char s[mem];

        for (map<string,double>::iterator it=c.vars.begin();it!=c.vars.end();it++)
        {
            out->print((*it).first);
            out->print("=");
            memset(s,0,mem);
            sprintf(s,"%lf",(*it).second);
            out->println(s);
        }
    }

    //funzioni usate nei polinomi
    double Calculator::p2(double x) //x*x
    {
        return x*x;
    }

    double Calculator::p3(double x) //x*x*x
    {
        return x*x*x;
    }

    double Calculator::p4(double x)
    {
        double d=x*x;
        return d*d;
    }

    double Calculator::p5(double x)
    {
        double d=x*x;
        return d*d*x;
    }

    double Calculator::p6(double x)
    {
        double d=x*x;

        return d*d*d;
    }

    double Calculator::p7(double x)
    {
        double d=x*x;

        return d*d*d*x;
    }

    double Calculator::p8(double x)
    {
        double d=x*x;
        d*=d;
        return d*d;

    }

    double Calculator::p9(double x)
    {
        double d=x*x;
        d*=d;
        return d*d*x;
    }

    double Calculator::p10(double x)
    {
        double d1=x*x;
        double d=d1*d1;
        d*=d;
        return d*d1;
    }


    double Calculator::abs(double x)
    {
        return fabs(x);
    }

    double Calculator::sgn(double x)
    {
        if (x>0) return 1;
        else if (x<0) return -1;
        return 0;
    }

    /**analizzatore lessicale*/
    void Calculator::GetToken() throw (ParseException) {

        char ch=0;

        cur_sym.string_value.clear();
        cur_sym.number_value=0;

        try {

            ch=input_stream->GetNextChar();
            //salta gli spazi
            while (isspace(ch)) {
                ch=input_stream->GetNextChar();
            }

            switch (ch) {
            case 0:
                cur_sym.token=END;
                return;
            case ';':
            case '*':
            case '/':
            case '+':
            case '-':
            case '(':
            case ')':
            case '=':
            case '"':
            case ',':

                cur_sym.token=Token(ch);
                cur_sym.string_value.push_back(ch);
                return;

                //controlla se il token � un numero
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '.':

                // input_stream->PutBack();

                // ch=input_stream->GetNextChar();

                try {

                    while (isdigit(ch) || ch=='.') {
                        cur_sym.string_value.push_back(ch);
                        ch=input_stream->GetNextChar();
                    }

                } catch (EndOfInput e) {
                }


                input_stream->PutBack();

                sscanf(cur_sym.string_value.c_str(),"%lf",&cur_sym.number_value);
                cur_sym.token=NUMBER;
                return;

            default:
                //� un nome

                if (isalpha(ch)) {

                    try {

                        while (isalnum(ch)) {
                            cur_sym.string_value.push_back(ch);
                            ch=input_stream->GetNextChar();
                        }

                    } catch (EndOfInput& e) {
                    }

                    cur_sym.token=NAME;

                    input_stream->PutBack();

                    return;

                } else {
                    string e="bad token:";
                    e.push_back(ch);
                    throw ParseException(e);
                }
            }

        } catch (EndOfInput e) {

            cur_sym.token=END;
            return;
        }

    }

    //acquisisce una espressione primaria (numero o simbolo)
    double Calculator::GetPrimary(bool next_token) throw (ParseException) {

        if (next_token) GetToken();

        switch (cur_sym.token) {

        case NUMBER: {

            double v=cur_sym.number_value;
            GetToken();
            return v;
        }

        case NAME: {

            string vname=cur_sym.string_value;

            //ricerca fra i nomi di funzioni
            SINGLE_ARG_FUNC f=sing_var_functions[vname];

            if (f) return EvalFunc(f);

            DOUBLE_ARG_FUNC fd=double_var_functions[vname];

            if (fd) return EvalFunc(fd);

            if (vname=="input") return GetUserInput();

            //funzioni con argomento stringa
            STR_FUNC f1=str_str_functions[vname];

            if (f1) return EvalFunc(f1);

            //comandi
            CMD_FUNC f2=cmd_functions[vname];

            if (f2)
            {
                EvalFunc(f2);
                return 0;
            }

            //nome di variabile
            double& v=vars[vname];

            GetToken();

            if (cur_sym.token==ASSIGN) {
                //assegna il valore ad una variabile o ne inizializza una nuova
                try
                {
                    CheckSymbol(vname);
                }
                catch (ReservedNameException& ex)
                {
                    string er="reserved name:";
                    er.append(vname);
                    throw ParseException(er);
                }

                v=EvalExpr(true);
            }

            return v;
        }

        case MINUS:
            //meno unario
            return -GetPrimary(true);
        case LP: {
            double e=EvalExpr(true);
            if (cur_sym.token!=RP) {
                //manca la parentesi di chiusura
                throw ParseException("')' expected");
            }
            GetToken(); //eat ')'
            return e;
        }
        default:
            throw ParseException("primary expected");
        }
    }

    /**acquisizione dell'input da parte dell'utente*/
    double Calculator::GetUserInput() throw (ParseException)
    {
        if (!user_input) throw ParseException("user input not defined.");

        GetToken();

        if (cur_sym.token!=LP) throw ParseException("'(' expected");

        char ch=input_stream->GetNextChar();

        //salta gli spazi
        while (isspace(ch)) {
            ch=input_stream->GetNextChar();
        }

        string r;

        if (ch==')')
        {
            //chiamata senza argomenti es. a=input();
            r="?";
        }
        else
        {
            //chiamata con il messaggio es. a=input("ins a:");
            if (ch==QUOT) {
                //l'argomento � di tipo stringa
                r=GetString();
            }
        }

        GetToken();

        if (cur_sym.token!=RP) throw ParseException("')' expected");

        return user_input->GetDouble(r);
    }

    //valuta una funzione
    double Calculator::EvalFunc(SINGLE_ARG_FUNC f) throw (ParseException,DivideByZeroException) {
        //valuta una funzione di una singola variabile
        GetToken();
        if (cur_sym.token!=LP) throw ParseException("'(' expected");
        double e=EvalExpr(true);
        if (cur_sym.token!=RP) {
            //manca la parentesi di chiusura
            throw ParseException("')' expected");
        }
        GetToken(); //eat ')'
        return f(e);
    }

    //valuta una funzione con due parametri
    double Calculator::EvalFunc(DOUBLE_ARG_FUNC f) throw (ParseException,DivideByZeroException)
    {
        GetToken();
        if (cur_sym.token!=LP) throw ParseException("'(' expected");
        double p1=EvalExpr(true); //primo argomento
        if (cur_sym.token!=COMMA) {
            //manca la parentesi di chiusura
            throw ParseException("',' expected in argument list.");
        }
        double p2=EvalExpr(true);//secondo argomento
        if (cur_sym.token!=RP) {
            //manca la parentesi di chiusura
            throw ParseException("')' expected");
        }
        GetToken(); //eat ')'

        return f(p1,p2);

    }

    void Calculator::EvalFunc(CMD_FUNC f) throw (ParseException)
    {
        GetToken();

        if (cur_sym.token!=LP) throw ParseException("'(' expected");
        GetToken();
        if (cur_sym.token!=RP) throw ParseException("')' expected");

        f(output_stream,*this);
    }


    /**valuta una funzione che accetta un argomento stringa*/
    double Calculator::EvalFunc(STR_FUNC f) throw (ParseException) {

        GetToken();
        if (cur_sym.token!=LP) throw ParseException("'(' expected");

        char ch=input_stream->GetNextChar();
        //salta gli spazi
        while (isspace(ch)) {
            ch=input_stream->GetNextChar();
        }

        if (ch==QUOT) {
            //l'argomento � di tipo stringa
            string r=GetString();

            GetToken();
            if (cur_sym.token!=RP) throw ParseException("')' expected");

            f(r,output_stream);

            return 0;

        } else {
            //argomento numerico da convertire in stringa
            input_stream->PutBack();
            double e=EvalExpr(true);
            if (cur_sym.token!=RP) {
                //manca la parentesi di chiusura
                throw ParseException("')' expected");
            }
            GetToken(); //eat ')'
            //trasforma il risultato in stringa

            char val[64];
            memset(val,0,64);
            sprintf(val,"%lf",e);

            string r;
            r.append(val);
            f(r,output_stream);

            return e;

        }
    }

    string Calculator::GetString() throw (ParseException) {
        string res="";
        bool bescape=false;

        try {

            char ch=input_stream->GetNextChar();

            while (ch!='"' || bescape) {
                if (ch=='\\' && bescape) bescape=true;
                else {
                    bescape=false;
                    res.push_back(ch);
                }

                ch=input_stream->GetNextChar();
            }

            return res;
        } catch (EndOfInput& e) {
            throw ParseException("unterminated string");
        }
    }

    //acquisisce un termine (term +/- term)
    double Calculator::GetTerm(bool next_token) throw (ParseException,DivideByZeroException) {
        double left=GetPrimary(next_token);

        for (;;) {
            switch (cur_sym.token) {
            case MUL:
                left *= GetPrimary(true);
                break;
            case DIV:
                if (double d=GetPrimary(true)) {
                    left/=d;
                    break;
                } else throw DivideByZeroException();
            default:
                return left;
            }
        }
    }

    double Calculator::EvalExpr(bool next_token) throw (ParseException,DivideByZeroException) {

        double left=GetTerm(next_token);

        for (;;) {
            switch (cur_sym.token) {
            case PLUS:

                left+=GetTerm(true);
                break;

            case MINUS:

                left-=GetTerm(true);
                break;

            default:

                return left;
            }
        }
    }

    void Calculator::SetOutput(OutputStream* o) {
        output_stream=o;
    }

    /**Valuta una espressione matematica e restituisce il risultato*/
    double Calculator::Evaluate(const string& expr) throw (ParseException,DivideByZeroException) {

        StringInputStream is(expr);

        input_stream=&is;

        return EvalExpr(true);
    }

    /**Esegue un programma (sequenza di espressioni matematiche).
      Il risultato del programma viene ottenuto interrogando la tabella delle variabili*/
    void Calculator::Run(InputStream* istr,OutputStream* ostr,UserInput* uinput,bool trace=false) throw (ParseException,DivideByZeroException) {

        if (!istr) throw ParseException("Input stream not defined");
        if (!ostr) throw ParseException("Output stream not defined");

        SetOutput(ostr);

        input_stream=istr;

        user_input=uinput;

        GetToken();

        if (trace)
        {
            int st_counter=0;
            const size_t mem=32;
            char s[mem];
            double r;


            while(cur_sym.token!=END) {

                memset(s,0,mem);
                sprintf(s,"%d",++st_counter);
                ostr->print("statement ");
                ostr->print(s);
                ostr->println(":");

                r=EvalExpr(false);

                ostr->print("eval=");
                memset(s,0,mem);
                sprintf(s,"%lf",r);
                ostr->println(s);

                GetToken();
                if (cur_sym.token==END_OF_STATEMENT) GetToken();
            }

        }
        else
        {

            while(cur_sym.token!=END) {

                EvalExpr(false);
                GetToken();
                if (cur_sym.token==END_OF_STATEMENT) GetToken();
            }

        }

    }

    /**Ottiene la lista delle funzioni con singolo parametro*/
    void Calculator::ListSingleArgFunctions(list<string>& func_list)
    {

        for (map<string,SINGLE_ARG_FUNC>::iterator it=sing_var_functions.begin();it!=sing_var_functions.end();it++)
        {
            func_list.push_back((*it).first);
        }
    }

    /**Ottiene la lista della funzioni con due parametri*/
    void Calculator::ListDoubleArgFunctions(list<string>& funct_list)
    {
        for (map<string,DOUBLE_ARG_FUNC>::iterator it=double_var_functions.begin();it!=double_var_functions.end();it++)
        {
            funct_list.push_back((*it).first);
        }
    }

    /**Ottiene la lista dei comandi*/
    void Calculator::ListCmdFunctions(list<string>& funct_list)
    {
        for (map<string,CMD_FUNC>::iterator it=cmd_functions.begin();it!=cmd_functions.end();it++)
        {
            funct_list.push_back((*it).first);
        }
    }

    /**Ottiene la lista di tutte le variabili definite*/
    void Calculator::ListVars(map<string,double>& mvar_list)
    {
        for (map<string,double>::iterator it=vars.begin();it!=vars.end();it++)
        {
            mvar_list.insert((*it));
        }
    }

}; //namespace
