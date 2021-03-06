/*******************************
 Formatatore e parser numerico
Code by Leonardo Berti (c) 2009
********************************/

#include "util/format.h"
#include <cstring>
#include <tchar.h>
#include <ctype.h>
#include <cstdlib>
#include <cstdio>

namespace util {

    ParseException::ParseException(const TCHAR* msg) {
        message = msg;
    }

    const TCHAR* ParseException::GetMsg() const {
        return message;
    }

    NumberFormat::NumberFormat(const Locale& loc) {
        dec_sep = *loc.GetDecimalPoint();
        th_sep = *loc.GetThousandsSep();
        SetPrecision(8);

    }

    NumberFormat::NumberFormat(const TCHAR* dec_sep, const TCHAR* th_sep) {
        if (dec_sep) this->dec_sep = *dec_sep;
        else this->dec_sep = TEXT('.');

        if (th_sep) this->th_sep = *th_sep;
        else this->th_sep = TEXT('\b');
        SetPrecision(8);

    }

    //crea un NumberFormat che non usa il separatore per le migliaia

    NumberFormat::NumberFormat(const TCHAR* dec_sep) {
        if (dec_sep) this->dec_sep = *dec_sep;
        else this->dec_sep = TEXT('.');

        th_sep = TEXT('\b');
        SetPrecision(8);

    }

    NumberFormat::~NumberFormat() {
    }

    /**
     *Analizza una stringa che rappresenta un numero intero o floating point
     *Toglie eventuali spazi
     *@return la stringa senza spazi e caratteri di tabulazione
     *@throw ParseException se la rappresentazione del numero non è corretta
     */
    const TCHAR* NumberFormat::PurgeString(const TCHAR* s) throw (ParseException) {

        if (!s) throw ParseException("cannot parse null");

        memset(temp, 0, sizeof (TCHAR) * MAX_CHARS);

        size_t len = strlen(s);

        if (len == 0) throw ParseException("cannot parse null string");

        size_t i;
        size_t j;
        bool dec_sep_found = false;
        bool ac_sign = true;

        TCHAR a;

        j = 0;

        if (len > MAX_CHARS) len = MAX_CHARS;

        a = s[0];

        if (!(isdigit(a) || a == TEXT('+') || a == TEXT('-') || a == TEXT(' '))) throw ParseException(s);

        for (i = 0; i < len; ++i) {

            a = s[i];

            if (isalpha(a)) throw ParseException(s);

            else if (a == dec_sep) {
                if (dec_sep_found) throw ParseException(s);
                temp[j++] = '.';
                dec_sep_found = true;
            }
            else if (a == TEXT(' ') || a == TEXT('\b') || a == TEXT('\n') || a == TEXT('\r')) {
                //salta
            } else if ((a == TEXT('+') || a == TEXT('-')) && ac_sign) {
                temp[j++] = a;
                ac_sign = false;
            } else if (isdigit(a)) {
                temp[j++] = a;
                ac_sign = false;
            } else {
                throw ParseException(s);
            }
        }

        return temp;
    }

    int NumberFormat::ParseInt(const TCHAR* text) throw (ParseException) {
        return atoi(PurgeString(text));
    }

    float NumberFormat::ParseFloat(const TCHAR* text) throw (ParseException) {
        return atof(PurgeString(text));
    }

    double NumberFormat::ParseDouble(const TCHAR* text) throw (ParseException) {
        return atof(PurgeString(text));
    }

    int NumberFormat::SafeParseInt(const TCHAR* text) {
        try {
            return ParseInt(text);
        }        catch (ParseException pe) {
            return 0;
        }
    }

    float NumberFormat::SafeParseFloat(const TCHAR* text) {
        try {
            return ParseFloat(text);
        }        catch (ParseException pe) {
            return 0.0f;
        }
    }

    double NumberFormat::SafeParseDouble(const TCHAR* text) {
        try {
            return ParseDouble(text);
        }        catch (ParseException pe) {
            return 0.0;
        }
    }

    //usata internamente:src � una stringa che rappresenta correttamente un numero

    void NumberFormat::FormatNumber(TCHAR* dest, const TCHAR* src, size_t size) {

        memset(dest, 0, sizeof (TCHAR) * size);

        int i, j;
        int len = strlen(src);
        TCHAR a;
        bool has_dec_point = false;
        bool found_dec_point = false;
        int thcnt = 0;

        //controlla se � un numero decimale
        for (i = 0; i < len; ++i) {
            if (src[i] == TEXT('.')) {
                has_dec_point = true;
                break;
            }
        }

        j = 0;

        if (has_dec_point) {
            //numero decimale
            for (i = len - 1; i >= 0; --i) {
                a = src[i];
                if (a == TEXT('.')) {
                    dest[j++] = dec_sep;
                    found_dec_point = true;
                } else {
                    dest[j++] = a;

                    if (found_dec_point) {
                        thcnt = thcnt + 1;
                        if (thcnt == 3) {
                            thcnt = 0;
                            if (th_sep != TEXT('\b') && i > 0) dest[j++] = th_sep;
                        }
                    }
                }
            }

        } else {

            //intero
            for (i = len - 1; i >= 0; --i) {

                a = src[i];
                dest[j++] = a;
                thcnt = thcnt + 1;
                if (thcnt == 3) {
                    thcnt = 0;
                    if (th_sep != TEXT('\b') && i > 0) dest[j++] = th_sep;
                }

            }

        }

        //inverte il buffer destinazione
        int sz = j / 2;

        for (i = 0; i < sz; ++i) {
            a = dest[i];
            dest[i] = dest[j - 1 - i];
            dest[j - 1 - i] = a;
        }
    }

    //imposta i decimali da visualizzare (rende la precisione precedente)

    int NumberFormat::SetPrecision(int prec) {
        int p = precision;

        if (prec >= 0) {
            memset(fmt, 0, 16 * sizeof (TCHAR));

            const char* p1 = "%";

            sprintf(fmt, "%s.%df", p1, prec);

            precision = prec;
        }

        return p;

    }

    //restituisce la precisione attuale

    int NumberFormat::GetPrecision() {
        return precision;
    }

    const TCHAR* NumberFormat::format(TCHAR* dest, int n, size_t size) {
        if (!dest) return 0;
        if (size == 0) return 0;

        memset(temp, 0, sizeof (TCHAR) * MAX_CHARS);

        sprintf(temp, "%d", n);

        FormatNumber(dest, temp, size);

        return dest;
    }

    const TCHAR* NumberFormat::format(TCHAR* dest, float n, size_t size) {

        if (!dest) return 0;
        if (size == 0) return 0;

        memset(temp, 0, sizeof (TCHAR) * MAX_CHARS);

        sprintf(temp, fmt, n);

        FormatNumber(dest, temp, size);

        return dest;

    }

    const TCHAR* NumberFormat::format(TCHAR* dest, double n, size_t size) {
        if (!dest) return 0;

        memset(temp, 0, sizeof (TCHAR) * MAX_CHARS);

        sprintf(temp, fmt, n);

        FormatNumber(dest, temp, size);

        return dest;

    }
};
