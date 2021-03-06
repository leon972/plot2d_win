/********************************
locales
Code by L.Berti 2009
*********************************/

#include <tchar.h>
#include "util/localedef.h"

namespace util {

    Locale::Locale(const TCHAR* dec_point,
                   const TCHAR* th_sep,
                   const TCHAR* currency_symbol,
                   const TCHAR* currency_dec_sep,
                   const TCHAR* country,
                   const TCHAR* language,
                   int dt_fmt) {

        this->dec_point=dec_point;
        this->th_sep=th_sep;
        this->currency_symbol=currency_symbol;
        this->country=country;
        this->language=language;
        this->dt_fmt=dt_fmt;
    }

    Locale::Locale()
    {
        this->dec_point=0;
        this->th_sep=0;
        this->currency_symbol=0;
        this->country=0;
        this->language=0;
        this->dt_fmt=0;
    }

    /**carattere usato come separatore decimale*/
    const TCHAR* Locale::GetDecimalPoint() const {
        return dec_point;
    }
    /**carattere usato come separatore delle migliaia*/
    const TCHAR* Locale::GetThousandsSep() const {
        return th_sep;
    }
    /**carattere usato come simbolo della valuta*/
    const TCHAR* Locale::GetCurrencySymbol() const {
        return currency_symbol;
    }
    /**carattere usato come punto decimale nella valuta*/
    const TCHAR* Locale::GetCurrencyDecSep() const {
        return currency_dec_sep;
    }
    /**Restituisce l'id del paese (ISO 3166 2-letter code)*/
    const TCHAR* Locale::GetCountry() const {
        return country;
    }
    /**Restituisce la lingua (ISO 639 code)*/
    const TCHAR* Locale::GetLanguage() const {
        return language;
    }
    /**Restituisce il formato data*/
    int Locale::GetDateFormatType() const {
        return dt_fmt;
    }

    Locale Locale::GetLocale(int country) throw (LocaleNotFoundException) {
        const TCHAR* dec_point;
        const TCHAR* th_sep;
        const TCHAR* currency_symbol;
        const TCHAR* currency_dec_sep;
        const TCHAR* chcountry; //ISO 3 letter
        const TCHAR* language; //ISO 3 letter
        int dt_fmt;

        switch (country) {
        case ITALY:

            dec_point=TEXT(",");
            th_sep=TEXT(".");
            currency_symbol=TEXT("€"); //0x80
            currency_dec_sep=TEXT(",");
            dt_fmt=DT_D_M_Y;
            chcountry=TEXT("ITA");
            language=TEXT("ita");

            break;

        case FRANCE:

            dec_point=",";
            th_sep=".";
            currency_symbol=TEXT("€"); //0x80
            currency_dec_sep=TEXT(",");
            dt_fmt=DT_D_M_Y;
            chcountry=TEXT("FRA");
            language=TEXT("fra");

            break;

        case GERMANY:

            dec_point=",";
            th_sep=".";
            currency_symbol=TEXT("€"); //0x80
            currency_dec_sep=TEXT(",");
            dt_fmt=DT_D_M_Y;
            chcountry=TEXT("DEU");
            language=TEXT("deu");

            break;
        case UK:

            dec_point=".";
            th_sep=",";
            currency_symbol=TEXT("€");
            currency_dec_sep=TEXT(",");
            dt_fmt=DT_M_D_Y;
            chcountry=TEXT("GBR");
            language=TEXT("eng");

            break;

        case US:

            dec_point=TEXT(".");
            th_sep=TEXT(",");
            currency_symbol=TEXT("$");
            currency_dec_sep=TEXT(",");
            dt_fmt=DT_M_D_Y;
            chcountry=TEXT("USA");
            language=TEXT("eng");

            break;

        case CHINA:

            dec_point=TEXT(".");
            th_sep=TEXT(",");
            currency_symbol=TEXT("?"); //da definire
            currency_dec_sep=TEXT(",");
            dt_fmt=DT_Y_M_D;
            chcountry=TEXT("CHN");
            language=TEXT("zho");
            break;

        case JAPAN:
        {
            dec_point=TEXT(".");
            th_sep=TEXT(",");

            currency_symbol=TEXT("?"); //simbolo ASCII dello Yen
            currency_dec_sep=TEXT(",");
            dt_fmt=DT_Y_M_D;
            chcountry=TEXT("JPN");
            language=TEXT("jpn");

            }


            break;

        case CANADA:
            dec_point=TEXT(".");
            th_sep=TEXT(",");
            currency_symbol=TEXT("$"); //simbolo ASCII dello Yen
            currency_dec_sep=TEXT(",");
            dt_fmt=DT_D_M_Y;
            chcountry=TEXT("CAN");
            language=TEXT("end");
            break;

        case CANADA_FRENCH:

            dec_point=TEXT(",");
            th_sep=TEXT(" ");
            currency_symbol=TEXT("$"); //simbolo ASCII dello Yen
            currency_dec_sep=TEXT(",");
            dt_fmt=DT_Y_M_D;
            chcountry=TEXT("CAN");
            language=TEXT("fra");

            break;

        case KOREA:

            dec_point=TEXT(".");
            th_sep=TEXT(",");
            currency_symbol=TEXT("?");
            currency_dec_sep=TEXT(",");
            dt_fmt=DT_Y_M_D;
            chcountry=TEXT("KOR");
            language=TEXT("kor");

            break;

        case TAIWAN:

            dec_point=TEXT(".");
            th_sep=TEXT(",");
            currency_symbol=TEXT("NT$");
            currency_dec_sep=TEXT(",");
            dt_fmt=DT_Y_M_D;
            chcountry=TEXT("TWN");
            language=TEXT("zho");

            break;

        default:

            throw LocaleNotFoundException();

        }//fine switch

        Locale loc(dec_point,th_sep,currency_symbol,currency_dec_sep,chcountry,language,dt_fmt);

        return loc;
    }

};
