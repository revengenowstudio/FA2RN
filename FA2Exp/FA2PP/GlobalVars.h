#pragma once

#include <INI.h>
#include <Helper/CompileTime.h>

class CLoading;
class CFinalSunDlg;
class CMapData;

class GlobalVars
{
public:
    static constexpr reference<char, 0x72A738, 260> const ExePath{}; // Where FA2.exe locates
    static constexpr reference<char, 0x72A530, 260> const MapPath{}; // Where current map locates 
    static constexpr reference<char, 0x72A634, 260> const FilePath{}; // Where mix files locates
    
	static constexpr reference<bool, 0x5CE3B8> const ShouldLoadYRFiles{};
	static constexpr reference<bool, 0x5D32AC> const ShouldSupportYR{};

    static constexpr reference<CMapData, 0x72CBF8> const CMapData{};

    class Dialog
    {
    public:
        static constexpr reference<CLoading*, 0x7EE030> const CLoading{};
        static constexpr reference<CFinalSunDlg*, 0x7EDF24> const CFinalSunDlg{};
    };

    class INIFiles
    {
    public:
        static constexpr reference<INIClass, 0x7EDDD8> const Rules{}; // rulesmd.ini / rules.ini
        static constexpr reference<INIClass, 0x7EDCA8> const Art{}; // artmd.ini / art.ini
        static constexpr reference<INIClass, 0x7ED960> const Turtorial{}; // turtorial.ini
        static constexpr reference<INIClass, 0x7EDA78> const Sound{}; // soundmd.ini / sound.ini
        static constexpr reference<INIClass, 0x7ED848> const Eva{}; // evamd.ini / eva.ini
        static constexpr reference<INIClass, 0x7ED730> const Theme{}; // thememd.ini / theme.ini
        static constexpr reference<INIClass, 0x7EDB90> const AI{}; // aimd.ini / ai.ini
        static constexpr reference<INIClass, 0x7ED3E8> const Temperate{}; // temperatmd.ini / temperat.ini
        static constexpr reference<INIClass, 0x7ED2D0> const Snow{}; // snowmd.ini / snow.ini
        static constexpr reference<INIClass, 0x7ED1B8> const Urban{}; // urbanmd.ini / urban.ini
        static constexpr reference<INIClass, 0x7ED0A0> const NewUrban{}; // urbannmd.ini
        static constexpr reference<INIClass, 0x7ECF88> const Lunar{}; // lunarmd.ini
        static constexpr reference<INIClass, 0x7ECE70> const Desert{}; // desertmd.ini
        static constexpr reference<INIClass, 0x7ED618> const FAData{}; // FAData.ini
        static constexpr reference<INIClass, 0x7ED500> const FALanguage{}; // FALanguage.ini
        static constexpr reference<INIClass, 0x7ACC80> const CurrentDocument{}; // Current map file, without update
    };


};