#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "lwtk/lwtkdefs.h"
#include "lwtk/lwtk.h"
#include "lwtk/exception.h"
#include "util/format.h"
#include <string>
#include <list>

using namespace lwtk;
using namespace std;

enum FUNCTION_TYPE {
    FT_CARTESIAN=0,
    FT_POLAR=1,
    FT_PARAMETRIC=2
};

//editor della funzione
struct FUNCT_GUI {

    Panel* panel;
    TextField* txt;
    TextField* txt1; //usato per le funzioni parametriche
    TextField* txtStep;
    TextField* txtInt1; //dominio
    TextField* txtInt2;
    Label* rangeLabel;
    Label* stepLabel;
    Label* label;
    ComboBox* ftype;
    Button* btuRemove;
};

struct FUNCT_DEF {
    string f1; //funzione es. y=f(x)
    string f2; //usata solo per funzioni parametriche tipo x=f(y) e y=f(t)
    double minx,maxx; //dominio di definizione
    double step; //passo per il calcolo
    FUNCTION_TYPE type;
};

//listener che viene invocato quando si esce dalla definizione funzioni
class FunctionsListener {
public:
    virtual void FunctionsEdited()=0;

};

class FunctionEditor:public ActionEvent,
            public WindowEvents,
            public ComboBoxEvents {
private:

    static const unsigned MAX_FUNCTIONS=4;//numero massimo di funzioni visualizzabili contemporaneamente
    static const int WIN_WIDTH=640;
    static const int WIN_HEIGHT=480;
    static const int TXT_INPUT_WIDTH=400; //larghezza testo input funzione
    static const int TXT_INPUT_WIDTH2=180; //larghezza testo input per funzioni parametriche
    static const int TXT_INPUT_WIDTH3=60; //testo input dominio e step
    static const int PANEL_WIDTH=638; //dimensioni pannello funzioni
    static const int PANEL_HEIGHT=90;

    list<string> functions_defs;
    Window* editor_window;
    Window* parent;
    list<FUNCT_GUI> functions;
    list<FUNCT_DEF> *ret_funct_def;
    Application* app;
    int wheight;
    FunctionsListener* listener;
    NumberFormat* numberFormat;

    void CreateGUI();
    void AddFunctionPanel();
    void ToggleLayout(FUNCT_GUI* panel,bool parametric,bool show_range);
    void GrabInterval(FUNCT_DEF* fd,const FUNCT_GUI& fg);
    void SetDefaultInterval(FUNCT_GUI* fg,FUNCTION_TYPE ftype);


public:

    FunctionEditor(Application* app,Window* parent,FunctionsListener* lst);
    ~FunctionEditor();

    void Show(list<FUNCT_DEF>* functs);

    void GetDefs(list<FUNCT_DEF>* defs);

    void ActionPerformed(Widget* src);

    void ComboListDblClick(ComboBox* src);

    void ListDropDown(ComboBox* src);

    void ItemSelected(ComboBox* src);

    void SetPositionRelativeTo(Window* w);

    /**La finestra viene attivata*/
    virtual void WindowActivated(Window* src);
    /**La finestra viene chiusa*/
    virtual void WindowClosed(Window* src);
    /**Evento che si verifica subito prima di chiudere la finestra.
     Se restituisce true chiude la finestra , altrimenti ne impedisce la chiusura*/
    virtual bool WindowClosing(Window* src);
    /**La finestra viene deattivata*/
    virtual void WindowDeactivated(Window* src);
    /**La finestra viene viene ripristinata*/
    virtual void WindowDeiconified(Window* src);
    /**La finestra � stata visualizzata a tutto schermo*/
    virtual void WindowMaximized(Window* src);
    /**La finestra viene ridotta ad icona*/
    virtual void WindowIconified(Window* src);
    /**La finestra viene aperta*/
    virtual void WindowOpened(Window* src);
    /**La finestra viene ridimensionata*/
    virtual void WindowResize(Window* src,int client_width,int client_height);
    /**La finestra viene spostata*/
    virtual void WindowMove(Window* src,int x,int y);


};


#endif // FUNCTIONS_H_INCLUDED
