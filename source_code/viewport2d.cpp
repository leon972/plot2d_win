/***************************************
Trasformazioni da coordinate logiche a
fisiche per un viewport 2D
Code by Leonardo Berti (c) 2009
*****************************************/

#include "geom/viewport2d.h"

using namespace std;

namespace geom {

    Viewport2D::Viewport2D(double phys_width,double phys_height,PHYSX_AXIS_DIR x_axis,PHYSY_AXIS_DIR y_axis) throw (Viewport2DException) {
        if (phys_width<=0 || phys_height<=0) throw Viewport2DException();

        x_dir = (x_axis==RIGHT_TO_LEFT ? -1 : 1);
        y_dir = (y_axis==TOP_DOWN ? -1 : 1);

        Wf=phys_width;
        Hf=phys_height;
        Wf2=Wf*0.5;
        Hf2=Hf*0.5;
        //inizialmente imposta una trasformazione neutra
        SetViewport(0,0,Wf,Hf);
    }

    void Viewport2D::SetViewport(double orig_lx,double orig_ly,double logic_width,double logic_height) throw (Viewport2DException) {
        if (logic_width<=0 || logic_height<=0) throw Viewport2DException();

        Wl=logic_width;
        Hl=logic_height;
        Oxl=orig_lx;
        Oyl=orig_ly;
        flx=Wf/Wl;
        fly=Hf/Hl;
        lfx=Wl/Wf;
        lfy=Hl/Hf;
    }

    void Viewport2D::SetViewportRect(double left,double bottom,double right,double top) throw (Viewport2DException) {
        double lw,lh,ox,oy;

        lw=right-left;
        lh=top-bottom;
        ox=left+lw*0.5;
        oy=bottom+lh*0.5;

        SetViewport(ox,oy,lw,lh);
    }

//trasforma da coordinare fisiche a logiche
    double Viewport2D::GetLogicX(double phys_x) {
        return x_dir*(phys_x-Wf2)*lfx+Oxl;
    }

    double Viewport2D::GetLogicY(double phys_y) {
        return y_dir*(phys_y-Hf2)*lfy+Oyl;
    }

//trasforma da coordinate logiche a fisiche
    double Viewport2D::GetPhysX(double logic_x) {
        return x_dir*(logic_x-Oxl)*flx+Wf2;
    }

    double Viewport2D::GetPhysY(double logic_y) {
        return y_dir*(logic_y-Oyl)*fly+Hf2;
    }

    //trasforma da unit� fisiche a logiche
    double Viewport2D::ToLogicUnitsX(double phys_len) {
        return phys_len*lfx;
    }

    double Viewport2D::ToLogicUnitsY(double phys_len) {
        return phys_len*lfy;
    }

    //da unit� lotgiche a fisiche
    double Viewport2D::ToPhysUnitsX(double logic_len) {
        return logic_len*flx;
    }

    double Viewport2D::ToPhysUnitsY(double logic_len) {
        return logic_len*fly;
    }

    void Viewport2D::SetPhysWidth(double fw) throw (Viewport2DException) {
        if (fw<=0) throw Viewport2DException();

        Wf=fw;
        Wf2=fw*0.5;
    }

    void Viewport2D::SetPhysHeight(double fh) throw (Viewport2DException) {
        if (fh<=0) throw Viewport2DException();

        Hf=fh;
        Hf2=fh*0.5;
        //aggiorna gli altri parametri
        SetViewport(Oxl,Oyl,Wl,Hl);
    }

    /**Esegue una traslazione
        il punto di coordinate logiche logic_x,logic_y andr� a coincidere con il
        pounto fisico phys_x phys_y
    */
    void Viewport2D::Pan(double logic_x,double logic_y,double phys_x,double phys_y) {
        // SetViewport(init_dragx-(x-Wf2)/zfx,(y-Hf2)/zfy+init_dragy,Wl,Hl);
        //Olx e Oly sono le variabili che vengono modificate
        SetViewport(logic_x-x_dir*(phys_x-Wf2)*lfx,logic_y-y_dir*(phys_y-Hf2)*lfy,Wl,Hl);
    }

    void Viewport2D::Zoom(double logic_len) throw (Viewport2DException) {

        double d=sqrt(Wl*Wl+Hl*Hl);

        double dx=(Wl*logic_len)/d;

        double nw=Wl+dx;

        SetViewport(Oxl,Oyl,nw,Hl*nw/Wl);

    }

    double Viewport2D::GetLogicWidth() {
        return Wl;
    }

    double Viewport2D::GetLogicHeight() {
        return Hl;
    }

    double Viewport2D::GetPhysWidth() {
        return Wf;
    }

    double Viewport2D::GetPhysHeight() {
        return Hf;
    }

    double Viewport2D::GetLogicCenterX() {
        return Oxl;
    }

    double Viewport2D::GetLogicCenterY() {
        return Oyl;
    }

    double Viewport2D::GetLogicAspectRatio() {
        return Wl/Hl;
    }

    double Viewport2D::GetPhysAspectRatio() {
        return Wf/Hf;
    }

    /**Aggiunge un bookmark della posizione e zoom correnti
        restituisce l'id del bookmark da usare per il ripristino
        */
    int Viewport2D::AddBookmark()
    {
        int id=state_bookmarks.size()+1;

        VIEWPORT2D_STATE state;

        state.Wl=Wl;
        state.Hl=Hl;
        state.Oxl=Oxl;
        state.Oyl=Oyl;

        state_bookmarks[id]=state;

        return id;
    }

    /**Ripristina una posizione salvata*/
    void Viewport2D::RestoreBookmark(int bookmark) throw (Viewport2DException)
    {
        map<int,VIEWPORT2D_STATE>::iterator it=state_bookmarks.find(bookmark);

        if (it==state_bookmarks.end()) throw Viewport2DException();

        VIEWPORT2D_STATE state=(*it).second;

        SetViewport(state.Oxl,state.Oyl,state.Wl,state.Hl);

    }

    /**Rimouve un bookmark*/
    void Viewport2D::RemoveBookmark(int bookmark) throw (Viewport2DException)
    {
        std::map<int,VIEWPORT2D_STATE>::iterator it=state_bookmarks.find(bookmark);

        if (it==state_bookmarks.end()) throw Viewport2DException();

        state_bookmarks.erase(it);
    }

    /**Elimina tutti i bookmark*/
    void Viewport2D::ClearBookmarks()
    {
       state_bookmarks.clear();
    }
};
