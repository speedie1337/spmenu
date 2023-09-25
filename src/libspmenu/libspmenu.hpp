#ifndef LIBSPMENU_HPP
#define LIBSPMENU_HPP

#include <iostream>

namespace spmenu {
    using Integer = std::string;
    using String = std::string;
    using Bool = bool;
    class spmenuSettings {
        public:
            String userArguments{};
            String standardInput{};

            String promptText{}; // -p
            String preText{}; // -pt
            String inputText{}; // -It
            String normalModeText{}; // -nmt
            String insertModeText{}; // -imt
            String capsLockOnText{}; // -clon
            String capsLockOffText{}; // -clof
            String leftArrowText{}; // -la
            String rightArrowText{}; // -ra
            Integer lineHeight; // -mh
            Integer centerWidth; // -cw
            Integer Lines; // -l
            Integer Columns; // -g
            Integer minLines; // -ml
            Bool generateCache; // -gc, -ngc
            Integer maxCache; // -mc
            String cacheDir{}; // -cd
            Bool printIndex; // -ix, -nix
            Bool Fast; // -f
            Bool Incremental; // -r, -nr
            Bool requireMatch; // -rm, -nrm
            Bool markItems; // -ma, -nma
            Bool Fuzzy; // -F, -NF
            Bool Regex; // -R, -NR
            Bool Password; // -P, -nP
            Bool Indent; // -ip, -nip
            Bool colorItems; // -ci, -nci
            Bool Sgr; // -sgr, -nsgr
            Bool Alpha; // -a, -na
            Bool allowTyping; // -tp, -nt
            Bool allowOverrideLines; // -ol, -nol
            Bool allowOverrideColumns; // -oc, -noc
            Integer X; // -x
            Integer Y; // -y
            Integer W; // -z
            Integer H; // -ml
            Integer Preselect; // -n
            Integer borderWidth; // -bw
            Bool Sort; // -so, -nso
            String Priority{}; // -pri
            Bool caseSensitive; // -s, -ns
            Bool defaultMode; // -nm, -im
            Integer windowPosition; // -t, -b, -c
            Integer itemPosition; // -itt, -itb
            Integer imagePosition; // -it, -ib, -ic, -itc
            Bool hideMode; // -hm
            Bool hideMatchCount; // -hmc
            Bool hideLeftArrow; // -hla
            Bool hideRightArrow; // -hra
            Bool hideItem; // -hit
            Bool hidePrompt; // -hpr
            Bool hidePretext; // -hpt
            Bool hideInput; // -hip
            Bool hidePowerline; // -hpl
            Bool hideCaret; // -hc
            Bool hideHighlighting; // -hhl
            Bool hideImage; // -hi
            Bool hideCaps; // -hcl
            Bool Xrdb; // -xrdb, -nxrdb
            Integer Monitor; // -m
            Integer Embed; // -w
            String histFile{}; // -H
            String listFile{}; // -lf
            String configFile{}; // -cf
            String bindsFile{}; // -bf
            String themeFile{}; // -tm
            Integer imageGaps; // -ig
            Integer imageSize; // -is
            Bool imageType; // -di, -df
            Bool imageResize; // -ir, -nir
            Integer textPadding; // -txp
            Integer verticalPadding; // -lp
            Integer horizontalPadding; // -hp
            Integer verticalMargin; // -vem
            Integer horizontalMargin; // -hom
            Bool Managed; // -wm, -nwm
            Bool displayProtocol; // -x11, -wl
            Bool loadConfig; // -lcfg, -ncfg
            Bool loadBinds; // -lbi, -nlbi
            Bool loadTheme; // -ltm, -nltm
            String Font; // -fn
            String normalItemForegroundColor{}; // -nif
            String normalItemBackgroundColor{}; // -nib
            String normalNextItemForegroundColor{}; // -nnif
            String normalNextItemBackgroundColor{}; // -nnib
            String selectedItemForegroundColor{}; // -sif
            String selectedItemBackgroundColor{}; // -sib
            String normalItemPriorityForegroundColor{}; // -npf
            String normalItemPriorityBackgroundColor{}; // -npb
            String selectedItemPriorityForegroundColor{}; // -spf
            String selectedItemPriorityBackgroundColor{}; // -spb
            String promptForegroundColor{}; // -pfg
            String promptBackgroundColor{}; // -pbg
            String inputForegroundColor{}; // -ifg
            String inputBackgroundColor{}; // -ibg
            String preTextForegroundColor{}; // -ptfg
            String preTextBackgroundColor{}; // -ptbg
            String windowColor{}; // -mnbg
            String normalHighlightForegroundColor{}; // -nhf
            String normalHighlightBackgroundColor{}; // -nhb
            String selectedHighlightForegroundColor{}; // -shf
            String selectedHighlightBackgroundColor{}; // -shb
            String numberForegroundColor{}; // -nfg
            String numberBackgroundColor{}; // -nbg
            String modeForegroundColor{}; // -mfg
            String modeBackgroundColor{}; // -mbg
            String leftArrowForegroundColor{}; // -laf
            String leftArrowBackgroundColor{}; // -lab
            String rightArrowForegroundColor{}; // -raf
            String rightArrowBackgroundColor{}; // -rab
            String caretForegroundColor{}; // -cfc
            String caretBackgroundColor{}; // -cbg
            String borderColor{}; // -bc
            String sgr0Color{}; // -sgr0
            String sgr1Color{}; // -sgr1
            String sgr2Color{}; // -sgr2
            String sgr3Color{}; // -sgr3
            String sgr4Color{}; // -sgr4
            String sgr5Color{}; // -sgr5
            String sgr6Color{}; // -sgr6
            String sgr7Color{}; // -sgr7
            String sgr8Color{}; // -sgr8
            String sgr9Color{}; // -sgr9
            String sgr10Color{}; // -sgr10
            String sgr11Color{}; // -sgr11
            String sgr12Color{}; // -sgr12
            String sgr13Color{}; // -sgr13
            String sgr14Color{}; // -sgr14
            String sgr15Color{}; // -sgr15
    };

    std::string Run(spmenuSettings *settings);
    std::string generateArguments(spmenuSettings *settings);
    std::string addArgument(std::string Argument, std::string Value);
    std::string addArgument(std::string enabledArgument, std::string disabledArgument, Bool Value);
    Bool checkBool(bool);
}

#endif
