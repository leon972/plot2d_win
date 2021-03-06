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
 * Programma per il tracciamento del grafico di
 * funzioni matematiche di una varibile
 * Espresse in modo esplicito y=f(x),
 * in coordinate polari r=r(theta)
 * o in coordinate parametriche x=x(t),y=y(t)
 *
 * Code by Leonardo Berti (c) 2009-2011
 ********************************************/

#include <iostream>

/**
 *Utilizza il wrapper LWTK (lighweight window tool kit)
 *per la gestione della interfaccia grafica in Windows
 */
#include "lwtk/lwtkdefs.h"
#include "lwtk/lwtk.h"
#include "lwtk/exception.h"
#include "cmd_line.h"
/**
 *Formattatore numerico
 */
#include "util/format.h"
/**
 *Viewport
 */
#include "geom/viewport2d.h"
/**
 *Interprete di espressioni matematiche
 */
#include "calculator/calculator.h"
/**
 * Interfaccia grafica dell'editor di funzioni
 */
#include "functions.h" 
#include <cmath>

#define PLATFORM_WIN32

#ifdef PLATFORM_WIN32
#include "lwtkwin32/lwtkwin32.h"
#else
#ifdef PLATFORM_XWINDOW
//da fare
#endif
#endif

using namespace lwtk;
using namespace std;
using namespace util;
using namespace geom;

/**
 *Applicazione
 */
lwtk::Application* application = 0;

/**
 *Finestra principale che contiene l'area in cui viene disegnato il grafico delle funzioni
 */
class PlotWindow : public PaintEvent,

public ActionEvent,
public KeyEvents,
public FocusEvents,
public MouseMoveEvents,
public MouseEvents,
public MouseRolloverEvents,
public FunctionsListener {
private:

    FunctionEditor* functEditor;
    Window* window;
    Panel* plotPanel;
    Panel* btuPanel;
    Button *btuPlot, *btuExit, *btuZoomIn, *btuZoomOut;
    Button *btuApply, *btuReset;
    TextField *txStartX, *txEndX, *txStartY, *txEndY;
    Label *lbStartX, *lbEndX, *lbStartY, *lbEndY;
    Label *lbCurX, *lbCurY;
    NumberFormat* nf;
    Font* plotFont;
    Viewport2D* viewport;
    int initial_pos;
    MenuBar* mainMenu;
    list<FUNCT_DEF> functions; //funzioni da disegnare
    calculator::Calculator calc;

    bool mouse_pressed1; //mouse drag
    bool mouse_pressed2;
    double init_dragx, init_dragy; //posizione logica iniziale del drag (PAN)
    double init_zoom_dragx, init_zoom_dragy; //posizione logica iniziale del drag (ZOOM)
    double init_lw, init_lh; //larghezza e altezza logiche all'inizio dello zoom

    double gridx, gridy;
    static const double grid_min = 40; //minima dimensione fisica griglia (pixel)
    static const double grid_max = 250; //massima dimensione fisica griglia (pixel)

    //dimensioni finestra
    static const int wwidth = 640;
    static const int wheight = 480;
    //dimensioni area del grafico
    static const int plotw = 620;
    static const int ploth = 300;
    static const int btuw = 90;
    static const int btuh = 32;
    static const COLOR plot_background = CL_NAVY;
    static const COLOR plot_grid_color = CL_TURQUOISE;
    static const COLOR plot_color = CL_WHITE;
    static const int lbw = 48;
    static const int txw = 60;

    Application* app;

    /**
     * Esegue lo zoom-in del grafico 
     * @param delta_pixel distanza acquisita tramite trascinamento del mouse che definisce
     * il fattore di zoom
     */
    void ZoomInViewport(double delta_pixel) {
        try {
            viewport->Zoom(-viewport->ToLogicUnitsX(delta_pixel));
            UpdateGridMetrics();
        } catch (Viewport2DException e) {
            window->MsgBox("Zoom Error", MSG_ICO_EXCLAMATION,"Zoom error!");
        }
    }

    /**
     * Esegue lo zoom-out del viewport grafico
     * @param delta_pixel distanza acquisita con il mouse
     */
    void ZoomOutViewport(double delta_pixel) {

        try {

            viewport->Zoom(viewport->ToLogicUnitsX(delta_pixel));
            UpdateGridMetrics();

        } catch (Viewport2DException e) {
            window->MsgBox("Zoom Error", MSG_ICO_EXCLAMATION, "Zoom error!");
        }
    }

    /**
     * Imposta le dimensioni standard di un pulsante
     * @param b
     */
    void MkBtu(Button* b) {
        b->SetWidth(btuw);
        b->SetHeight(btuh);
    }

    /**
     * Posizione un pulsante
     * @param b
     * @param ref
     */
    void PosBtu(Button* b, Button* ref) {
        b->Move(ref->GetLeft() + ref->GetWidth() + 5, ref->GetTop());
    }

    /**
     * Posizione un widget rispetto al precedente
     * @param w
     * @param ref
     */
    void PosLine(Control* w, Control* ref) {
        w->Move(ref->GetLeft() + ref->GetWidth() + 5, ref->GetTop());
    }


    /**
     * Aggiorna le dimensioni della griglia.
     */
    void UpdateGridMetrics() {

        double l = viewport->ToPhysUnitsX(gridx);
        double ratio = gridy / gridx;

        if (l < grid_min) {
            gridx = viewport->ToLogicUnitsX(grid_max);
            gridy = ratio*gridx;
        } else if (l > grid_max) {
            gridx = viewport->ToLogicUnitsX(grid_min);
            gridy = ratio*gridx;
        }

        l = viewport->ToPhysUnitsX(gridy);

        if (l < grid_min) {
            gridy = viewport->ToLogicUnitsY(grid_max);
        } else if (l > grid_max) {
            gridy = viewport->ToLogicUnitsY(grid_min);
        }

    }

    /**
     * Crea il menu della finestra principale
     */
    void CreateMenu() {

        mainMenu = app->GetNewMenuBar();
        Menu* m = app->GetNewMenu(TEXT("File"));
        MenuItem* mi;
        mi = app->GetNewMenuItem(TEXT("Functions editor"));
        mi->SetAction("define_fun");
        mi->AddActionEvent(this);
        m->AddItem(mi);       
        m->AddSeparator();
        mi = app->GetNewMenuItem(TEXT("Exit"));
        mi->SetAction("exit");
        mi->AddActionEvent(this);
        m->AddItem(mi);
        mainMenu->AddMenu(m);
        m = app->GetNewMenu(TEXT("Help"));
        mi = app->GetNewMenuItem(TEXT("Instructions"));
        mi->SetAction("display_builtin");
        mi->AddActionEvent(this);
        m->AddItem(mi);
        mi = app->GetNewMenuItem("About");
        mi->SetAction("about");
        mi->AddActionEvent(this);
        m->AddItem(mi);
        mainMenu->AddMenu(m);
        window->SetMenuBar(mainMenu);
    }

    /**
     * Invocata come call-back dal functions editor quando si
     * modifcano le funzioni
     */
    void FunctionsEdited() {
        functEditor->GetDefs(&functions);
    }

    /**
     * Crea l'interfaccia grafica (GUI) della finestra principale
     * La finestra principale è costituita da un'area in cui viene tracciato il grafico
     * delle funzioni nella parte alta e una serie di pulsanti per il controllo dello zoom e
     * del pan
     * Lo zoom e il pan possono essere ottenuti anche trascindando e tenendo premuti il tasto sinistro e destro
     * del mouse
     */
    void CreateGUI() {

        window = app->GetNewWindow(TEXT("Plot"), false, true, false);
        window->SetWidth(wwidth);
        window->SetHeight(wheight);
        window->AddPaintEvent(this);

        plotFont = app->GetNewFont(TEXT("Arial"), 8, false, false, false);

        btuPanel = app->GetNewPanel(wwidth - 30, 100, BORDER_ETCHED);
        btuPanel->Move(10, 310);

        btuPlot = app->GetNewButton("Plot");
        btuExit = app->GetNewButton("Exit");
        btuApply = app->GetNewButton("Apply");
        btuReset = app->GetNewButton("Reset");
        MkBtu(btuPlot);
        MkBtu(btuExit);
        MkBtu(btuApply);
        MkBtu(btuReset);

        btuPanel->Add(btuPlot);
        btuPanel->Add(btuExit);

        btuPlot->AddActionEvent(this);
        btuPlot->SetAction("plot");
        btuExit->AddActionEvent(this);
        btuExit->SetAction("exit");
        btuApply->AddActionEvent(this);
        btuApply->SetAction("apply");
        btuReset->SetAction("reset");
        btuReset->AddActionEvent(this);

        btuZoomIn = app->GetNewButton(TEXT("Zoom In"), TEXT("zoomin"));
        MkBtu(btuZoomIn);
        btuZoomIn->AddActionEvent(this);

        btuZoomOut = app->GetNewButton("Zoom Out", "zoomout");
        MkBtu(btuZoomOut);
        btuZoomOut->AddActionEvent(this);

        btuPlot->Move(10, 10);
        PosBtu(btuZoomIn, btuPlot);
        PosBtu(btuZoomOut, btuZoomIn);
        PosBtu(btuReset, btuZoomOut);
        PosBtu(btuExit, btuReset);

        btuPanel->Add(btuZoomIn);
        btuPanel->Add(btuZoomOut);
        btuPanel->Add(btuReset);
        //visualizzazione X,Y correnti
        lbCurX = app->GetNewLabel(TEXT(""), BORDER_SUNKEN);
        lbCurY = app->GetNewLabel(TEXT(""), BORDER_SUNKEN);

        lbCurX->SetBackground(RGB_COLOR(0, 0, 0xFF));
        lbCurY->SetBackground(RGB_COLOR(0, 0, 0xFF));
        lbCurX->SetForeground(CL_WHITE);
        lbCurY->SetForeground(CL_WHITE);

        lbCurX->SetWidth(90);
        lbCurY->SetWidth(90);

        lbCurX->SetText("X");
        lbCurY->SetText("Y");

        lbCurX->Move(480, 5);
        lbCurY->Move(480, 28);

        txStartX = app->GetNewTextField(8, TEXT_NUMERIC_FLOAT);
        txStartY = app->GetNewTextField(8, TEXT_NUMERIC_FLOAT);
        txEndX = app->GetNewTextField(8, TEXT_NUMERIC_FLOAT);
        txEndY = app->GetNewTextField(8, TEXT_NUMERIC_FLOAT);

        // pw->SetBackground(CL_WHITE);

        txStartX->SetWidth(txw);
        txEndX->SetWidth(txw);
        txStartY->SetWidth(txw);
        txEndY->SetWidth(txw);

        txStartX->AddFocusEvents(this);
        txEndX->AddFocusEvents(this);
        txStartY->AddFocusEvents(this);
        txEndY->AddFocusEvents(this);

        lbStartX = app->GetNewLabel("X start");
        lbEndX = app->GetNewLabel("X end");
        lbStartY = app->GetNewLabel("Y start");
        lbEndY = app->GetNewLabel("Y end");

        lbStartX->SetWidth(lbw);
        lbEndX->SetWidth(lbw);
        lbStartY->SetWidth(lbw);
        lbEndY->SetWidth(lbw);

        lbStartX->SetLeft(btuPlot->GetLeft());
        lbStartX->SetTop(btuPlot->GetTop() + btuPlot->GetHeight() + 20);

        //posiziona i controlli su una linea
        PosLine(txStartX, lbStartX);
        PosLine(lbEndX, txStartX);
        PosLine(txEndX, lbEndX);
        PosLine(lbStartY, txEndX);
        PosLine(txStartY, lbStartY);
        PosLine(lbEndY, txStartY);
        PosLine(txEndY, lbEndY);
        PosLine(btuApply, txEndY);

        btuApply->SetHeight(txEndY->GetHeight());

        btuPanel->Add(lbStartX);
        btuPanel->Add(lbStartY);
        btuPanel->Add(txStartX);
        btuPanel->Add(txStartY);

        btuPanel->Add(lbEndX);
        btuPanel->Add(lbEndY);
        btuPanel->Add(txEndX);
        btuPanel->Add(txEndY);
        btuPanel->Add(btuApply);
        btuPanel->Add(lbCurX);
        btuPanel->Add(lbCurY);

        plotPanel = app->GetNewPanel(plotw, ploth, BORDER_SUNKEN);

        plotPanel->SetForeground(plot_color);
        plotPanel->Move(10, 5);
        plotPanel->AddPaintEvent(this);
        //intercetta i movimenti del mouse
        plotPanel->AddMouseMoveEvents(this);
        //intercetta le azioni del mouse
        plotPanel->AddMouseEvents(this);
        plotPanel->AddMouseRolloverEvents(this);

        window->Add(plotPanel);
        window->Add(btuPanel);
        window->AddKeyEvents(this);

        //crea il menu
        CreateMenu();

        plotPanel->SetBackground(plot_background);

    }

public:

    /**
     * Crea una istanza della finestra di tracciamento del grafico
     * @param app
     */
    PlotWindow(Application* app) throw (Exception) {

        if (app == 0) throw Exception("Non � stata impostata l'applicazione.");

        this->app = app;

        nf = new NumberFormat(app->GetSystemInstance()->GetLocale());

        CreateGUI();

        functEditor = new FunctionEditor(app, window, this);

        //dimensioni fisiche in pixel del grafico
        viewport = new Viewport2D(plotw, ploth, geom::LEFT_TO_RIGHT, geom::TOP_DOWN);

        //unit� logiche della griglia
        gridx = 10;
        gridy = 10;
        viewport->SetViewport(0, 0, 50, 50);
        UpdateGridMetrics();

        try {
            //bookmark posizione iniziale
            initial_pos = viewport->AddBookmark();
        } catch (Viewport2DException e) {
            window->MsgBox("Error", MSG_ICO_EXCLAMATION, "Error setting initial bookmark position.");
        }

        mouse_pressed1 = mouse_pressed2 = false;
        
        window->Show(W_QUIT_ON_CLOSE);               
    }

    /**
     * Rilascia la memoria allocata
     */
    virtual ~PlotWindow() {
        if (nf) delete nf;
        nf = 0;
        if (viewport) delete viewport;
        viewport = 0;
        if (functEditor) delete functEditor;
    }

    void FirstStart() {

        window->MsgBox(window->GetTitle(),MSG_ICO_INFORMATION,"Open the functions editor then choose the function type (cartesian,polar or parametric) and write the function expression (eg. 3*x*x-cos(x))");
        showEditor();
    }

    /**
     * Apre l'editor di funzioni
     * @param src
     * @return 
     */
    void showEditor() {

        functEditor->SetPositionRelativeTo(window);
        functEditor->Show(&functions);

    }

    /**
     * Metodo CALLBACK che intercetta l'input dell'utente sui vari widget dell'interfaccia
     * @param src widget che ha invocato il metodo
     */
    void ActionPerformed(Widget* src) {

        const TCHAR* cmd = src->GetAction();

        //uscita
        if (strcmp(cmd, "exit") == 0) {

            window->Close();

            //zoom-in
        } else if (strcmp(cmd, "zoomin") == 0) {

            ZoomInViewport(20);
            plotPanel->Refresh();

            //zoom-out
        } else if (strcmp(cmd, "zoomout") == 0) {

            ZoomOutViewport(20);
            plotPanel->Refresh();

            //about dialog
        } else if (strcmp(cmd, "about") == 0) {
            window->MsgBox("About", MB_ICONINFORMATION, "Plot - Copyright by Leonardo Berti (c) 2009");

            //visualizza lista delle funzioni built-int
        } else if (strcmp(cmd, "display_builtin") == 0) {
            window->MsgBox("Instructions", MB_ICONINFORMATION, "Open the functions editor from menu File->Function Editor, choose the function type (cartesian,polar or parametric) then write your function.In the function expression you can use built-in math functions,for example: 3*cos(0.5*x)-tanh(x/2) \n\nClick and drag with left mouse button on the graphic panel to pan,\nor drag using the right mouse button to zoom.\n\n\rAvailable built-in functions:\n \nsqrt square root\nsin sine\ncos cosine\ntan tangent\nacos arc-cosine\nasin arc-sine\natan arc-tangent\nceil smallest int greater or equal to x\nfloor largest integer value less than or equal to x\ncosh hyperbolic cosine\nsinh hyperbolic sine\ntanh hyperbolic tangent\nexp e raised to the xth power\nfabs absolute value\nlog or ln natural logarithm\nlog10 base-10 logarithm\npow x raised to y ex.usage: pow(x,0.2)\npolinomial p1,p2,p3..p10 ex. p3(x)=x*x*x");

            //disegna il grafico
        } else if (strcmp(cmd, "plot") == 0) {

            if (functions.empty()) {

                window->MsgBox(window->GetTitle(),MSG_ICO_INFORMATION,"First you need to define a function to plot!");
                showEditor();

            }
            else {
                plotPanel->Refresh();
            }

            //applica le impostazioni
        } else if (strcmp(cmd, "apply") == 0) {
            try {

                double start_x, start_y, end_x, end_y;
                double w, h;

                start_x = nf->ParseDouble(txStartX->GetText());
                start_y = nf->ParseDouble(txStartY->GetText());
                end_x = nf->ParseDouble(txEndX->GetText());
                end_y = nf->ParseDouble(txEndY->GetText());

                w = end_x - start_x;
                h = end_y - start_y;

                viewport->SetViewport(start_x + w / 2, start_y + h / 2, w, h);
                UpdateGridMetrics();
                plotPanel->Refresh();

            } catch (util::ParseException& pe) {
                window->MsgBox("Error", MSG_ICO_INFORMATION, "Invalid input:%s", pe.GetMsg());
            }
        } else if (strcmp(cmd, "reset") == 0) {
            try {
                viewport->RestoreBookmark(initial_pos);
                //unit� logiche iniziali della griglia
                gridx = 10;
                gridy = 10;
                UpdateGridMetrics();
                plotPanel->Refresh();
            } catch (Viewport2DException e) {
                window->MsgBox("Bookmark error", MSG_ICO_EXCLAMATION, "Error setting initial position");
            }
        } else if (strcmp(cmd, "define_fun") == 0) {

            showEditor();
        }
    }

    /**
     * Metodo CALLBACK che intercetta i movimenti del mouse
     * trascinando con il tasto sinistro o destro si ottiene lo ZOOM
     * o il PAN dell'area di disegno
     * @param x
     * @param y
     */

    void MouseMove(int x, int y) {

        try {

            if (mouse_pressed1) {

                //PAN
                viewport->Pan(init_dragx, init_dragy, x, y);
                UpdateGridMetrics();

                plotPanel->Refresh();

            } else if (mouse_pressed2) {
                //ZOOM
                double dx = viewport->GetLogicX(x) - init_zoom_dragx;
                double dy = viewport->GetLogicY(y) - init_zoom_dragy;

                double d = sqrt(dx * dx + dy * dy) / 20;

                if (dy > 0) viewport->Zoom(d);
                else viewport->Zoom(-d);
                UpdateGridMetrics();

                plotPanel->Refresh();
            }

            TCHAR s[10];
            nf->SetPrecision(6);
            lbCurX->SetText(nf->format(s, viewport->GetLogicX(x), 10));
            lbCurY->SetText(nf->format(s, viewport->GetLogicY(y), 10));

        } catch (Viewport2DException e) {
            mouse_pressed2 = false;
            mouse_pressed1 = false;
            window->MsgBox("Viewport Error", MSG_ICO_EXCLAMATION, "ZOOM/PAN Error!");
        }

    }

    /**
     * Traccia il grafico di una funzione valutando per ogni valore della varibile indipendente il valore della funzione
     * con l'inteprete di espressioni
     * @param g interfaccia grafica di output
     * @param f funzione
     * @param c colore del grafico
     */
    void DrawFunction(Graphics* g, const FUNCT_DEF& f, COLOR c) {

        double x1 = 0;
        double x2 = viewport->GetPhysWidth();
        double x, y;
        double yd;
        double xprev, yprev;
        double rho;
        double tstart;
        double tend;
        double tstep;
        double astep;
        const double aconv = M_PI / 180;
        string f1;
        string f2;

        double stepx = 1;

        g->SetDrawColor(c);

        try {

            switch (f.type) {

                //grafico cartesioni y=f(x)
                case FT_CARTESIAN:

                    //definizione funzione es.  "3*x+2"
                    f1 = f.f1;

                    if (f1 == "") return;

                    xprev = x1;
                    calc.SetVar("x", viewport->GetLogicX(x1));
                    try {
                        yprev = viewport->GetPhysY(calc.Evaluate(f1));
                        x = x1;
                    } catch (calculator::DivideByZeroException ex) {
                    }

                    while (x <= x2) {

                        x += stepx;
                        //imposta la x nel calculator
                        calc.SetLastVar(viewport->GetLogicX(x));

                        try {

                            y = calc.Evaluate(f1);

                            yd = viewport->GetPhysY(y);

                            g->DrawLine(static_cast<int> (xprev), static_cast<int> (yprev), static_cast<int> (x), static_cast<int> (yd));

                            xprev = x;
                            yprev = yd;

                        } catch (calculator::DivideByZeroException& ex) {
                            //asintoto verticale
                            g->SetDrawColor(CL_WHITE);
                            g->DrawLine(static_cast<int> (x), 0, static_cast<int> (x), static_cast<int> (viewport->GetPhysHeight()));
                            g->SetDrawColor(c);
                        }

                    }

                    break;

                    //polare r=r(theta)
                case FT_POLAR:

                    f1 = f.f1;

                    if (f1 == "") return;

                    calc.SetVar("t", 0); //theta
                    try {
                        rho = calc.Evaluate(f1);
                    } catch (calculator::DivideByZeroException& ex) {
                    }

                    xprev = viewport->GetPhysX(rho * cos(x1 * aconv));
                    yprev = viewport->GetPhysY(rho * sin(x1 * aconv));

                    if (f.maxx != 0 && f.step != 0) {
                        tstart = f.minx;
                        tend = f.maxx;
                        tstep = f.step;

                    } else {
                        tstart = 0;
                        tend = M_PI * 2;
                        tstep = aconv * 0.5;
                    }

                    for (x1 = tstart; x1 <= tend; x1 += tstep) {

                        try {
                            calc.SetLastVar(x1); //theta
                            rho = calc.Evaluate(f1);

                            x = viewport->GetPhysX(rho * cos(x1));
                            y = viewport->GetPhysY(rho * sin(x1));

                            g->DrawLine(static_cast<int> (xprev), static_cast<int> (yprev), static_cast<int> (x), static_cast<int> (y));

                            xprev = x;
                            yprev = y;
                        } catch (calculator::DivideByZeroException& ex) {
                        }



                    }

                    break;

                    //parametrica x=x(t) y=y(t)
                case FT_PARAMETRIC:

                    f1 = f.f1;
                    f2 = f.f2;

                    if (f1 == "") {
                        window->MsgBox("Error", MSG_ICO_EXCLAMATION, "x(t) is not defined!");
                        return;
                    }

                    if (f2 == "") {
                        window->MsgBox("Error", MSG_ICO_EXCLAMATION, "y(t) is not defined!");
                        return;
                    }

                    if (f.maxx != 0 && f.step != 0) {
                        tstart = f.minx;
                        tend = f.maxx;
                        tstep = f.step;

                    } else {
                        tstart = -200;
                        tend = 200;
                        tstep = 0.5;
                    }

                    calc.SetVar("t", tstart);

                    try {

                        x = viewport->GetPhysX(calc.Evaluate(f1));
                        y = viewport->GetPhysY(calc.Evaluate(f2));

                    } catch (calculator::DivideByZeroException& ex) {
                    }



                    xprev = x;
                    yprev = y;

                    for (x1 = tstart; x1 <= tend; x1 += tstep) {
                        calc.SetLastVar(x1);

                        try {

                            x = viewport->GetPhysX(calc.Evaluate(f1));
                            y = viewport->GetPhysY(calc.Evaluate(f2));

                            g->DrawLine(static_cast<int> (xprev), static_cast<int> (yprev), static_cast<int> (x), static_cast<int> (y));

                            xprev = x;
                            yprev = y;

                        } catch (calculator::DivideByZeroException& ex) {
                        }
                    }

                    break;

            }//switch

        } catch (calculator::ParseException& ex) {
            window->MsgBox("Parse Error", MSG_ICO_EXCLAMATION, "Error in function %s : %s", f1.c_str(), ex.GetErrorMessage().c_str());
        }
    }

    /**
     * Disegna il grafico di tutte le funzioni che l'utente ha definito nell'editor
     * @param g
     */
    void DrawFunctions(Graphics* g) {
        COLOR cols[5] = {CL_YELLOW, CL_GREEN, CL_LIGHTMAGENTA, CL_LIGHTGRAY, CL_GOLD};
        int r = 0;

        for (list<FUNCT_DEF>::iterator it = functions.begin(); it != functions.end(); it++) {
            DrawFunction(g, *it, cols[(r++) % 5]);
        }
    }

    /**
     * Disegna il grafico di una funzione generica usando direttamente il puntatore
     * della funzione senza usare l'interprete (usata per test)
     * @param g interfaccia grafica di oputput
     * @param pfny puntatore alla funzione da tracciare
     */
    void DrawPlot(Graphics* g, double (*pfny) (double)) {

        if (!pfny) return;

        double x1 = 0;
        double x2 = viewport->GetPhysWidth();
        double x, y;
        double yd;
        double xprev, yprev;

        double stepx = 1;

        g->SetDrawColor(CL_WHITE);

        xprev = x1;
        yprev = viewport->GetPhysY(pfny(viewport->GetLogicX(x1)));
        x = x1;

        while (x <= x2) {

            x += stepx;
            y = pfny(viewport->GetLogicX(x));

            yd = viewport->GetPhysY(y);

            g->DrawLine(static_cast<int> (xprev), static_cast<int> (yprev), static_cast<int> (x), static_cast<int> (yd));

            xprev = x;
            yprev = yd;

        }
    }

    /**
     * Disegna la griglia usando i settaggi correnti
     * determina automaticamente le dimensioni della griglia in base allo zoom corrente
     * @param g
     */
    void DrawGrid(Graphics* g) {
        //griglia
        double x1, x2;
        double xc, xfc;
        double axis;
        double t;        
        double Hf = viewport->GetPhysHeight();
        double Wf = viewport->GetPhysWidth();

        const size_t lbsize = 16;
        int prec = 2;

        TCHAR lb[lbsize];

        g->SetDrawColor(CL_LIGHTCYAN);

        g->SetFont(plotFont);

        g->SetTextColor(CL_LIGHTYELLOW);

        x1 = (int) ((viewport->GetLogicX(0) - gridx) / gridx) * gridx;

        x2 = viewport->GetLogicX(viewport->GetPhysWidth()) + gridx;

        xc = x1;

        axis = viewport->GetPhysX(0);

        memset(lb, 0, sizeof (TCHAR) * lbsize);
        memset(lb, 0, sizeof (TCHAR) * lbsize);

        t = 1 / gridx;

        //calcola la precisione di rappresentazione in base alla griglia
        if (t > 0) {
            prec = static_cast<int> (1 + log10(t));
        } else prec = 2;

        if (prec < 2) prec = 2;
        else if (prec > 6) prec = 6;

        nf->SetPrecision(prec);

        //linee verticali
        while (xc < x2) {

            xfc = viewport->GetPhysX(xc);

            if (abs(xfc - axis) > 5) g->DrawLine(static_cast<int> (xfc), 0, static_cast<int> (xfc), static_cast<int> (Hf));

            g->WriteText(static_cast<int> (xfc - 20), static_cast<int> (Hf - 20), nf->format(lb, xc, lbsize));

            xc += gridx;
        }

        x1 = viewport->GetLogicY(0) + gridy;

        x2 = (int) ((viewport->GetLogicY(viewport->GetPhysHeight()) - gridy) / gridy) * gridy;

        xc = x2;

        axis = viewport->GetPhysY(0);

        t = 1 / gridx;

        if (t > 0) {
            prec = static_cast<int> (1 + log10(t));
        } else prec = 2;

        if (prec < 2) prec = 2;
        else if (prec > 6) prec = 6;

        nf->SetPrecision(prec);

        //linee orizzontali
        while (xc < x1) {

            xfc = viewport->GetPhysY(xc);

            if (abs(xfc - axis) > 5) g->DrawLine(0, static_cast<int> (xfc), static_cast<int> (Wf), static_cast<int> (xfc));

            g->WriteText(10, static_cast<int> (xfc - 20), nf->format(lb, xc, lbsize));

            xc += gridy;
        }

        //assi coordinati
        g->SetDrawColor(CL_ORANGE);

        double oxl, oyl;

        oxl = viewport->GetPhysX(0);
        oyl = viewport->GetPhysY(0);

        g->DrawLine(0, static_cast<int> (oyl), plotw, static_cast<int> (oyl));
        g->DrawLine(static_cast<int> (oxl), 0, static_cast<int> (oxl), ploth);
    }

    /**
     * Callback per eventi paint del plotPanel
     * Viene invocato ogni volta che viene ridisegnata la finestra
     * @param g
     */
    void Paint(Graphics* g) {

        if (!g) return;

        DrawGrid(g);

        //traccia i grafici delle funzioni
        DrawFunctions(g);
    }

    /**Tasto premuto*/
    int KeyDown(const KeyInfo* key_info) {
        if (key_info) {
            if (key_info->GetKeyCode() == K_PLUS) ActionPerformed(btuZoomIn);
            else if (key_info->GetKeyCode() == K_MINUS) ActionPerformed(btuZoomOut);
        }
        return 0;
    }

    /**Tasto rilasciato*/
    int KeyUp(const KeyInfo* key_info) {
        return 0;
    }

    /**Tasto ASCII premuto*/
    int KeyPressed(const KeyInfo* key_info) {
        return 0;
    }

    /**
     * Focus listener
     * @param src
     */
    void FocusLost(Widget* src) {
        if (src) src->SetBackground(CL_WHITE);
    }

    void FocusGained(Widget* src) {
        if (src) src->SetBackground(CL_YELLOW);
    }

    void MouseClicked(MouseInfo* mouse_info) {
    }

    void MouseDblClicked(MouseInfo* mouse_info) {
    }

    /**
     * Intercetta lo stato dei tasti del mouse
     * e cmabia lo stato PAN/ZOOM in base al tasto premuto
     * @param mouse_info
     */
    void MousePressed(MouseInfo* mouse_info) {

        //PAN
        if (mouse_info->GetButton() == LEFT_BUTTON) {
            init_dragx = viewport->GetLogicX(mouse_info->GetX());
            init_dragy = viewport->GetLogicY(mouse_info->GetY());
            mouse_pressed1 = true;
        }//ZOOM
        else if (mouse_info->GetButton() == RIGHT_BUTTON) {
            init_zoom_dragx = viewport->GetLogicX(mouse_info->GetX());
            init_zoom_dragy = viewport->GetLogicY(mouse_info->GetY());
            init_lw = viewport->GetLogicWidth();
            init_lh = viewport->GetLogicHeight();
            mouse_pressed2 = true;
        };
    }

    void MouseReleased(MouseInfo* mouse_info) {
        if (mouse_info->GetButton() == LEFT_BUTTON) mouse_pressed1 = false;
        else mouse_pressed2 = false;
    }

    //il mouse netra nell'area del grafico

    void MouseEntered(MouseInfo* mouse_info) {
        // plotPanel->SetBackground(CL_YELLOW);
        // plotPanel->Refresh();

    }

    //il mouse esce dall'area del grafico

    void MouseExited(MouseInfo* mouse_info) {
        mouse_pressed1 = false;
        mouse_pressed2 = false;

    }

};


/**
 * Entry point
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {


#ifndef CMD_LINE_MODE

    //interfaccia grafica

    try {
        
#ifdef PLATFORM_WIN32
        //Windows
        application = lwtk::Win32Application::GetInstance();

#endif

        PlotWindow ftw(application);

        application->Release();

        //rilascia la memoria allocata
        delete application;

    } catch (Exception& e) {
        cout << e.GetMsg() << endl;
    }

#else

    //test interprete con interfaccia command line
    CalculatorTextUI cmdLineUI;

    cmdLineUI.Start();

#endif

    return 0;
}
