/**
 * Test per interprete di espressioni matematiche
 * Command line mode
 * Permette la valutazione di espressioni o programmi
 * direttamente dalla linea di comando
 * Coded by Leonardo Berti (c) 2011
 *
 */

#ifndef CMD_LINE_H
#define	CMD_LINE_H

#include <iostream>
#include "calculator/calculator.h"

using namespace calculator;
using namespace std;


/**
 * Interfaccia testo per il test dell'interprete di espressioni
 * matematiche
 */
class CalculatorTextUI {

private:

    static const int LINE_WIDTH=60;
    static const int INPUT_EXPR=1;
    static const int INPUT_VAR=2;
    static const int EXIT_REQUEST=0;
    
    /**
     * Output linea di separazione
     * @return
     */
    void WriteSep();

    /**
     * Scrive una linea dentro un riquadro
     * @param text
     */
    void WriteBoxed(const string &text);

    /**
     *Visualizza il titolo introduttivo
     */
    void DisplayTitle();

    /**
     * Menu principale
     *
     */
    void MainMenu();

    /**
     *Chiede di immettere una espressione matematica
     *da interpretare
     */
    void EvaluateExpression();

    /**
     * Definisce una varibile
     * @return
     */
    void DefineVar();

    /**
     * Visualizza stato variabili
     * @return
     */
    void DumpState();

    /**
     * Attende la pressione di un tasto
     */
    void WaitKey();

    /**
     * Legge un valore numerico floating point dalla console
     */
    double ReadDouble();

    /**
     * Interprete espressioni
     */
    Calculator* calculator;

public:

    CalculatorTextUI();

    virtual ~CalculatorTextUI();

    void Start();

};


#endif	/* CMD_LINE_H */

