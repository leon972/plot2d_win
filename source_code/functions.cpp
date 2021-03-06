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

/*******************************************
Editor per la definizione delle funzioni
********************************************/

#include "functions.h"
#include <cmath>
#include <cstring>

using namespace lwtk;
using namespace std;

FunctionEditor::FunctionEditor(Application* app,Window* parent,FunctionsListener* lst) {

    ret_funct_def=0;
    listener=lst;
    this->app=app;
    this->parent=parent;
    numberFormat=new NumberFormat(app->GetSystemInstance()->GetLocale());

    CreateGUI();

}

FunctionEditor::~FunctionEditor() {
    if (numberFormat) {
        delete numberFormat;
        numberFormat=0;
    }
}

void FunctionEditor::AddFunctionPanel() {

    const int yctrl=54;

    if (functions.size()>=MAX_FUNCTIONS) {
        editor_window->MsgBox("Error",MSG_ICO_EXCLAMATION,TEXT("Cannot draw more than %d functions!"),MAX_FUNCTIONS);
        return;
    }

    FUNCT_GUI f;

    f.panel=app->GetNewPanel(PANEL_WIDTH,PANEL_HEIGHT,BORDER_ETCHED);

    f.txt=app->GetNewTextField();
    f.txt->SetWidth(TXT_INPUT_WIDTH);

    //txt1 � usato per la y delle funzioni parametriche
    f.txt1=app->GetNewTextField();
    f.txt1->SetWidth(TXT_INPUT_WIDTH2);
    f.txt1->Move(184,28);
    f.txt1->SetVisible(false);

    f.label=app->GetNewLabel("f(x)=");
    f.label->Move(2,5);
    f.label->SetWidth(260);
    f.label->SetForeground(CL_BLUE);

    f.panel->Add(f.label);

    f.txt->Move(2,28);
    f.panel->Add(f.txt);
    f.panel->Add(f.txt1);

    f.ftype=app->GetNewComboBox(0,10,DROPDOWN_READ_ONLY,false);
    f.ftype->SetWidth(160);
    f.ftype->AddElement(TEXT("cartesian y=f(x)"));
    f.ftype->AddElement(TEXT("polar r=r(t)"));
    f.ftype->AddElement(TEXT("parametric x=x(t); y=y(t)"));
    f.ftype->Move(450,28);
    f.ftype->AddComboBoxEvents(this);
    f.ftype->SetSelectedIndex(0);

    f.panel->Add(f.ftype);
    f.ftype->SetSelectedIndex(0);

    f.btuRemove=app->GetNewButton("Delete");
    f.btuRemove->Move(2,50);
    f.btuRemove->SetAction("delete_function");
    f.btuRemove->AddActionEvent(this);
    f.panel->Add(f.btuRemove);

    //dominio di definizione
    f.txtInt1=app->GetNewTextField(6,TEXT_NUMERIC_FLOAT);
    f.txtInt1->SetWidth(TXT_INPUT_WIDTH3);
    f.txtInt2=app->GetNewTextField(6,TEXT_NUMERIC_FLOAT);
    f.txtInt2->SetWidth(TXT_INPUT_WIDTH3);
    //passo usato per il calcolo
    f.txtStep=app->GetNewTextField(6,TEXT_NUMERIC_FLOAT);
    f.txtStep->SetWidth(TXT_INPUT_WIDTH3);

    f.rangeLabel=app->GetNewLabel("interval");
    f.rangeLabel->SetForeground(CL_BLUE);
    f.rangeLabel->Move(80,yctrl);

    f.txtInt1->Move(150,yctrl);
    f.txtInt2->Move(220,yctrl);

    f.stepLabel=app->GetNewLabel("step");
    f.stepLabel->SetForeground(CL_BLUE);

    f.stepLabel->Move(300,yctrl);

    f.txtStep->Move(350,yctrl);

    f.txtInt1->SetVisible(false);
    f.txtInt2->SetVisible(false);
    f.txtStep->SetVisible(false);
    f.rangeLabel->SetVisible(false);
    f.stepLabel->SetVisible(false);

    f.panel->Add(f.txtInt1);
    f.panel->Add(f.txtInt2);
    f.panel->Add(f.txtStep);
    f.panel->Add(f.rangeLabel);
    f.panel->Add(f.stepLabel);

    f.panel->SetTop(functions.size()*PANEL_HEIGHT);

    editor_window->Add(f.panel);

    functions.push_back(f);
}

void FunctionEditor::CreateGUI() {

    editor_window=app->GetNewWindow(TEXT("Function Editor"),false,true,false);
    editor_window->SetWidth(WIN_WIDTH);
    editor_window->SetHeight(WIN_HEIGHT);
    editor_window->AddWindowEvents(this);

    Panel* btuPanel=app->GetNewPanel(630,60,BORDER_ETCHED);

    Button* b=app->GetNewButton("Save","save",80,30);
    b->AddActionEvent(this);
    b->Move(10,2);

    btuPanel->Add(b);

    b=app->GetNewButton("Close","close",80,30);
    b->AddActionEvent(this);
    b->Move(100,2);
    btuPanel->Add(b);

    b=app->GetNewButton("New function","new",80,30);
    b->SetWidth(120);
    b->AddActionEvent(this);
    b->Move(200,2);
    btuPanel->Add(b);
    btuPanel->SetHeight(40);

    btuPanel->Move(2,editor_window->GetHeight()-btuPanel->GetHeight()-40);

    editor_window->Add(btuPanel);

    AddFunctionPanel();
}

/**cambia il layout del panello a seconda se si deve visualizzare
una funzione parametrica o no
show_range: se impostato visualizza i campi per l'input del range e dello step
*/
void FunctionEditor::ToggleLayout(FUNCT_GUI* panel,bool parametric,bool show_range) {
    if (parametric) {
        panel->txt->SetWidth(TXT_INPUT_WIDTH2);
        panel->txt1->SetVisible(true);
    } else {
        panel->txt->SetWidth(TXT_INPUT_WIDTH);
        panel->txt1->SetVisible(false);
        panel->txt1->SetText("");
    }

    panel->txtInt1->SetVisible(show_range);
    panel->txtInt2->SetVisible(show_range);
    panel->txtStep->SetVisible(show_range);
    panel->rangeLabel->SetVisible(show_range);
    panel->stepLabel->SetVisible(show_range);
}

void FunctionEditor::Show(list<FUNCT_DEF>* functs) {
    editor_window->SetVisible(true);
    ret_funct_def=functs;
}

void FunctionEditor::GrabInterval(FUNCT_DEF* fd,const FUNCT_GUI& fg) {
    try {
        fd->minx=numberFormat->ParseDouble(fg.txtInt1->GetText());
    } catch (ParseException& pe) {
        if (string(fg.txtInt1->GetText()) != "") editor_window->MsgBox("Error",MSG_ICO_EXCLAMATION,"Invalid range min value!");
        fd->minx=0;
    }

    try {
        fd->maxx=numberFormat->ParseDouble(fg.txtInt2->GetText());
    } catch (ParseException& pe) {
        if (string(fg.txtInt2->GetText()) != "") editor_window->MsgBox("Error",MSG_ICO_EXCLAMATION,"Invalid range max value!");
        fd->maxx=0;
    }

    try {
        fd->step=numberFormat->ParseDouble(fg.txtStep->GetText());
    } catch (ParseException& pe) {
        if (string(fg.txtStep->GetText()) != "") editor_window->MsgBox("Error",MSG_ICO_EXCLAMATION,"Invalid step!");
        fd->step=0;
    }

    if (fd->maxx<=fd->minx && fd->maxx!=0 && fd->minx != 0) {
        editor_window->MsgBox("Error",MSG_ICO_EXCLAMATION,"Invalid range!");
    }
}

void FunctionEditor::GetDefs(list<FUNCT_DEF>* defs) {

    if (!defs) return;

    defs->clear();

    FUNCT_DEF fd;

    for (list<FUNCT_GUI>::iterator it=functions.begin();it!=functions.end();it++) {

        FUNCT_GUI fg=(*it);

        switch (fg.ftype->GetSelectedIndex()) {
        case 0:
        case -1:
            fd.type=FT_CARTESIAN;
            fd.f1=fg.txt->GetText();
            break;
        case 1:
            fd.type=FT_POLAR;
            fd.f1=fg.txt->GetText();
            GrabInterval(&fd,fg);
            break;
        case 2:

            fd.type=FT_PARAMETRIC;
            fd.f1=fg.txt->GetText();
            fd.f2=fg.txt1->GetText();
            GrabInterval(&fd,fg);
            break;
        }

        defs->push_back(fd);
    }
}

void FunctionEditor::SetPositionRelativeTo(Window* w) {
    editor_window->SetPositionRelativeTo(w);
}

void FunctionEditor::ActionPerformed(Widget* src) {

    if (!src) return;

    string cmd(src->GetAction());

    if (cmd=="save") {

        //esegue la chiamata call-back per inforare che le funzioni sono state modificate
        if (listener) listener->FunctionsEdited();

    } else if (cmd=="close") {
        editor_window->SetVisible(false);
    } else if (cmd=="new") {
        AddFunctionPanel();
    } else if (cmd=="delete_function") {
        for (list<FUNCT_GUI>::iterator it=functions.begin();it!=functions.end();it++) {
            if (dynamic_cast<Button*>(src)==((*it).btuRemove)) {
                (*it).txt->SetText("");
                (*it).txt1->SetText("");
                /*(*it).txtInt1->SetText("");
                (*it).txtInt2->SetText("");
                (*it).txtStep->SetText("");*/
                //editor_window->Remove((*it).panel);
            }
        }
    }
}

void FunctionEditor::ComboListDblClick(ComboBox* src) {

}

/**La lista viene visualizzata*/
void FunctionEditor::ListDropDown(ComboBox* src) {
}

void FunctionEditor::SetDefaultInterval(FUNCT_GUI* fg,FUNCTION_TYPE ftype) {

    TCHAR temp[64];
    memset(temp,0,sizeof(TCHAR)*64);
    switch (ftype) {
    case FT_POLAR:

        fg->txtInt1->SetText("0");
        fg->txtInt2->SetText(numberFormat->format(temp,M_PI*2,64));
        fg->txtStep->SetText(numberFormat->format(temp,M_PI/180,64));

        break;
    case FT_PARAMETRIC:
        fg->txtInt1->SetText("-200");
        fg->txtInt2->SetText("200");
        fg->txtStep->SetText(numberFormat->format(temp,0.5,64));

        break;
        case FT_CARTESIAN:

        fg->txtInt1->SetText("-20");
        fg->txtInt2->SetText("20");
        fg->txtStep->SetText(numberFormat->format(temp,0.5,64));

        break;
    }

}

/**Viene selezionato un elemento della lista*/
void FunctionEditor::ItemSelected(ComboBox* src) {

    for (list<FUNCT_GUI>::iterator it=functions.begin();it!=functions.end();it++) {
        if ((*it).ftype==src) {
            //aggiorna il pannello in base al tipo di selezione
            switch (src->GetSelectedIndex()) {
            case 1:
                //polare
                (*it).label->SetText("r(t)=                     (example: 3*t+2)");
                ToggleLayout(&(*it),false,true);
                SetDefaultInterval(&(*it),FT_POLAR);

                break;
            case 2:
                //parametrica
                (*it).label->SetText("y=y(t) x=x(t)              (example: 3*t+2 , 2-t+4)");
                ToggleLayout(&(*it),true,true);
                SetDefaultInterval(&(*it),FT_PARAMETRIC);

                break;
            default:
                //cartesiana
                (*it).label->SetText("f(x)=                      (example: 3*x+2)");
                ToggleLayout(&(*it),false,false);
                break;

            }
        }
    }
}

/**La finestra viene attivata*/
void FunctionEditor::WindowActivated(Window* src) {
}
/**La finestra viene chiusa*/
void FunctionEditor::WindowClosed(Window* src) {


}
/**Evento che si verifica subito prima di chiudere la finestra.
 Se restituisce true chiude la finestra , altrimenti ne impedisce la chiusura*/
bool FunctionEditor::WindowClosing(Window* src) {

    editor_window->SetVisible(false);

    GetDefs(ret_funct_def);


    //restituire false serve anon far distruggere la finestra
    //in fase di chiusura in modo da poterla rivisualizzare senza ricrearla da zero
    return false;
}

/**La finestra viene deattivata*/
void FunctionEditor::WindowDeactivated(Window* src) {
}
/**La finestra viene viene ripristinata*/
void FunctionEditor::WindowDeiconified(Window* src) {
}
/**La finestra � stata visualizzata a tutto schermo*/
void FunctionEditor::WindowMaximized(Window* src) {
}
/**La finestra viene ridotta ad icona*/
void FunctionEditor::WindowIconified(Window* src) {
}
/**La finestra viene aperta*/
void FunctionEditor::WindowOpened(Window* src) {
}
/**La finestra viene ridimensionata*/
void FunctionEditor::WindowResize(Window* src,int client_width,int client_height) {
}
/**La finestra viene spostata*/
void FunctionEditor::WindowMove(Window* src,int x,int y) {
}




