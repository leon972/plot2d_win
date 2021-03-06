/**
 * Interprete di espressioni matematiche
 * Command line mode
 * Permette la valutazione di espressioni o programmi
 * direttamente dalla linea di comando
 * Coded by Leonardo Berti (c) 2011
 *
 */

#include "cmd_line.h"
#include <iomanip>


CalculatorTextUI::CalculatorTextUI() {
    //istanzia il parser di espressioni
    calculator = new Calculator();
}

CalculatorTextUI::~CalculatorTextUI() {

    if (calculator) {
        delete calculator;
        calculator = 0;
    }
}

/**
 * Legge un valore numerico floating point dalla console
 */
double CalculatorTextUI::ReadDouble() {

    double r = 0;

    while (!(cin >> r)) {

        cout << "Inserire un numero valido!" << endl;
        cin.clear();
        cin.sync();
    }

    return r;
}

/**
 * Output linea di separazione
 * @return
 */
void CalculatorTextUI::WriteSep() {

    cout << "************************************************************" << endl;
}

/**
 * Scrive una linea dentro un riquadro
 * @param text
 */
void CalculatorTextUI::WriteBoxed(const string &text) {
    
    cout << "* ";
    cout.width(LINE_WIDTH - 3);
    cout << setiosflags (ios_base::left) << text;
    cout << "*" << endl;
}

/**
 *Visualizza il titolo introduttivo
 */
void CalculatorTextUI::DisplayTitle() {

    WriteSep();
    WriteBoxed("Test parser di espressioni matematiche");
    WriteBoxed("Copyright (c) 2009 by Leonardo Berti");
    WriteBoxed("");
    WriteSep();
}

/**
 * Menu principale
 */
void CalculatorTextUI::MainMenu() {

    char r;


    for (;;) {

        cin.clear();
        cin.sync();
        
        cout << endl;
        WriteSep();

        cout << " - Main Menu -" << endl;

        cout << "1 - Immetti una espressione da valutare"<<endl;
        cout << "2 - Definisci una variabile"<<endl;
        cout << "3 - Visualizza stato variabili"<<endl;
        cout << "0 - Esci";

        cout << endl;

        cout << "Seleziona:" << endl;

        if (!cin.get(r)) {

            cout << "Errore!" << endl;
            //annulla l'input
            cin.clear();
            cin.sync();
        }

        switch (r) {

            case '1':

                EvaluateExpression();
                break;

            case '2':

                DefineVar();
                break;

            case '3':

                DumpState();
                break;

            case '0':
                //esce dal menu
                return;
            default:
                cout << "Il valore immesso non e' valido!" << endl;

        }
    }
}

/**
 *Chiede di immettere una espressione matematica
 *da interpretare
 */
void CalculatorTextUI::EvaluateExpression() {

    for (;;) {

        cout << "Immettere una espressione matematica (esempio: (1+2)/(3*4-5)+cos(1.22)/sin(0.5))" << endl;
        cout << endl;

        string expression;

        if (!(cin >> expression)) {

            cout << "Errore input!" << endl;
            //annulla l'input
            cin.clear();
            cin.sync();
            break;
        }

        try {

            double r = calculator->Evaluate(expression);

            cout << endl << "Il risultato e' " << r << endl;
            WaitKey();
            break;

        } catch (ParseException &ex) {
            cerr << ex.GetErrorMessage() << endl;
        } catch (DivideByZeroException &ex) {
            cerr << "Divisione per zero!" << endl;
        } catch (...) {
            cerr << "Errore durante l'interpretazione dell'espressione.";
        }
    }
}

/**
 * Definisce una varibile
 * @return
 */
void CalculatorTextUI::DefineVar() {

    for (;;) {

        cout << "Immettere il nome della variabile da definire (esempio: x)" << endl;

        string var;
        
        if (!(cin>>var)) {

            cout << "Errore input!" << endl;
            //annulla l'input
            cin.clear();
            cin.sync();
            break;
        }

        cout << endl << "Immettere il valore della variabile " << var << endl;

        try {

            //setta lo stato della variabile
            calculator->SetVar(var, ReadDouble());
            cout << endl << "La variabile " << var << " ora puo' essere usata nelle espressioni." << endl;
            break;

        } catch (ReservedNameException &ex) {
            cerr << endl << var << "e' un nome riservato usato internamente, immettere un nome diverso!" << endl;
        }
    }
}

/**
 * Visualizza lo stato delle varibili
 */
void CalculatorTextUI::DumpState() {

    cout << endl << "Stato delle varibili:" << endl;

    map<string, double> state;

    calculator->ListVars(state);

    for (map<string, double>::iterator it = state.begin(); it != state.end(); it++) {

        cout << (*it).first << " = " << (*it).second << endl;
    }

    cout << endl;
}

void CalculatorTextUI::WaitKey() {

    //da fare
}

/**
 * Avvia l'interfaccia utente
 */
void CalculatorTextUI::Start() {

    DisplayTitle();
    MainMenu();

}
