/* libspmenu
 * spmenu C++ library
 *
 * See LICENSE file for copyright and license details.
 */

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <array>
#include "libspmenu.h"

std::string spmenu::addArgument(std::string Argument, std::string Value) {
    return " " + Argument + " \"" + Value + "\"";
}

std::string spmenu::addArgument(std::string enabledArgument, std::string disabledArgument, std::string Value) {
    if (disabledArgument.empty() || enabledArgument.empty()) {
        return "";
    } else if (Value.compare("True") && Value.compare("true") && Value.compare("False") && Value.compare("false") && Value.compare("0") && Value.compare("1")) {
        return "";
    }

    return " " + ((!Value.compare("True") || !Value.compare("true") || !Value.compare("1")) ? enabledArgument : disabledArgument);
}

std::string spmenu::generateArguments(spmenu::spmenuSettings *settings) {
    std::string argList;

    argList += (settings->promptText.empty()) ? "" : (spmenu::addArgument("--prompt", settings->promptText));
    argList += (settings->preText.empty()) ? "" : (spmenu::addArgument("--pretext", settings->preText));
    argList += (settings->inputText.empty()) ? "" : (spmenu::addArgument("--input", settings->inputText));
    argList += (settings->normalModeText.empty()) ? "" : (spmenu::addArgument("--normal-mode-text", settings->normalModeText));
    argList += (settings->insertModeText.empty()) ? "" : (spmenu::addArgument("--insert-mode-text", settings->insertModeText));
    argList += (settings->capsLockOnText.empty()) ? "" : (spmenu::addArgument("--caps-lock-on-text", settings->capsLockOnText));
    argList += (settings->capsLockOffText.empty()) ? "" : (spmenu::addArgument("--caps-lock-off-text", settings->capsLockOffText));
    argList += (settings->leftArrowText.empty()) ? "" : (spmenu::addArgument("--left-arrow-symbol", settings->leftArrowText));
    argList += (settings->rightArrowText.empty()) ? "" : (spmenu::addArgument("--right-arrow-symbol", settings->rightArrowText));
    argList += (settings->lineHeight.empty()) ? "" : (spmenu::addArgument("--line-height", settings->lineHeight));
    argList += (settings->centerWidth.empty()) ? "" : (spmenu::addArgument("--center-width", settings->centerWidth));
    argList += (settings->Lines.empty()) ? "" : (spmenu::addArgument("--lines", settings->Lines));
    argList += (settings->Columns.empty()) ? "" : (spmenu::addArgument("--columns", settings->Columns));
    argList += (settings->minLines.empty()) ? "" : (spmenu::addArgument("--min-lines", settings->minLines));
    argList += spmenu::addArgument("--generate-cache", "--no-generate-cache", settings->generateCache);
    argList += (settings->maxCache.empty()) ? "" : (spmenu::addArgument("--max-cache", settings->maxCache));
    argList += (settings->cacheDir.empty()) ? "" : (spmenu::addArgument("--cache-dir", settings->cacheDir));
    argList += spmenu::addArgument("--print-index", "--no-print-index", settings->printIndex);
    argList += spmenu::addArgument("--fast", "", settings->Fast);
    argList += spmenu::addArgument("--incremental", "--no-incremental", settings->Incremental);
    argList += spmenu::addArgument("--require-match", "--no-require-match", settings->requireMatch);
    argList += spmenu::addArgument("--mark-items", "--no-mark-items", settings->markItems);
    argList += spmenu::addArgument("--fuzzy", "--no-fuzzy", settings->Fuzzy);
    argList += spmenu::addArgument("--regex", "--no-regex", settings->Regex);
    argList += spmenu::addArgument("--password", "--no-password", settings->Password);
    argList += spmenu::addArgument("--indent", "--no-indent", settings->Indent);
    argList += spmenu::addArgument("--color-items", "--no-color-items", settings->colorItems);
    argList += spmenu::addArgument("--sgr", "--no-sgr", settings->Sgr);
    argList += spmenu::addArgument("--alpha", "--no-alpha", settings->Alpha);
    argList += spmenu::addArgument("--allow-typing", "--no-allow-typing", settings->allowTyping);
    argList += spmenu::addArgument("--override-lines", "--no-override-lines", settings->allowOverrideLines);
    argList += spmenu::addArgument("--override-columns", "--no-override-columns", settings->allowOverrideColumns);
    argList += (settings->X.empty()) ? "" : (spmenu::addArgument("--x-position", settings->X));
    argList += (settings->Y.empty()) ? "" : (spmenu::addArgument("--y-position", settings->Y));
    argList += (settings->W.empty()) ? "" : (spmenu::addArgument("--width", settings->W));
    argList += (settings->H.empty()) ? "" : (spmenu::addArgument("--min-lines", settings->H));
    argList += (settings->Preselect.empty()) ? "" : (spmenu::addArgument("--preselect", settings->Preselect));
    argList += (settings->borderWidth.empty()) ? "" : (spmenu::addArgument("--border-width", settings->borderWidth));
    argList += spmenu::addArgument("--sort", "--no-sort", settings->Sort);
    argList += (settings->Priority.empty()) ? "" : (spmenu::addArgument("--priority", settings->Priority));
    argList += spmenu::addArgument("--case-sensitive", "--case-insensitive", settings->caseSensitive);
    argList += spmenu::addArgument("--normal", "--insert", settings->defaultMode);
    argList += spmenu::addArgument("--hide-mode", "--show-mode", settings->hideMode);
    argList += spmenu::addArgument("--hide-match-count", "--show-match-count", settings->hideMatchCount);
    argList += spmenu::addArgument("--hide-left-arrow", "--show-left-arrow", settings->hideLeftArrow);
    argList += spmenu::addArgument("--hide-right-arrow", "--show-right-arrow", settings->hideRightArrow);
    argList += spmenu::addArgument("--hide-item", "--show-item", settings->hideItem);
    argList += spmenu::addArgument("--hide-prompt", "--show-prompt", settings->hidePrompt);
    argList += spmenu::addArgument("--hide-pretext", "--show-pretext", settings->hidePretext);
    argList += spmenu::addArgument("--hide-input", "--show-input", settings->hideInput);
    argList += spmenu::addArgument("--hide-powerline", "--show-powerline", settings->hidePowerline);
    argList += spmenu::addArgument("--hide-caret", "--show-caret", settings->hideCaret);
    argList += spmenu::addArgument("--hide-highlighting", "--show-highlighting", settings->hideHighlighting);
    argList += spmenu::addArgument("--hide-image", "--show-image", settings->hideImage);
    argList += spmenu::addArgument("--hide-caps", "--show-caps", settings->hideCaps);
    argList += spmenu::addArgument("--xrdb", "--no-xrdb", settings->Xrdb);
    argList += (settings->Monitor.empty()) ? "" : (spmenu::addArgument("--monitor", settings->Monitor));
    argList += (settings->Embed.empty()) ? "" : (spmenu::addArgument("--embed", settings->Embed));
    argList += (settings->histFile.empty()) ? "" : (spmenu::addArgument("--hist-file", settings->histFile));
    argList += (settings->listFile.empty()) ? "" : (spmenu::addArgument("--list-file", settings->listFile));
    argList += (settings->configFile.empty()) ? "" : (spmenu::addArgument("--config-file", settings->configFile));
    argList += (settings->bindsFile.empty()) ? "" : (spmenu::addArgument("--bind-file", settings->bindsFile));
    argList += (settings->themeFile.empty()) ? "" : (spmenu::addArgument("--theme-file", settings->themeFile));
    argList += (settings->imageGaps.empty()) ? "" : (spmenu::addArgument("--image-gaps", settings->imageGaps));
    argList += (settings->imageSize.empty()) ? "" : (spmenu::addArgument("--image-size", settings->imageSize));
    argList += spmenu::addArgument("--display-image", "--display-icons", settings->imageType);
    argList += spmenu::addArgument("--image-resize", "--no-image-resize", settings->imageResize);
    argList += (settings->textPadding.empty()) ? "" : (spmenu::addArgument("--text-padding", settings->textPadding));
    argList += (settings->verticalPadding.empty()) ? "" : (spmenu::addArgument("--vertical-padding", settings->verticalPadding));
    argList += (settings->horizontalPadding.empty()) ? "" : (spmenu::addArgument("--horizontal-padding", settings->horizontalPadding));
    argList += (settings->verticalMargin.empty()) ? "" : (spmenu::addArgument("--vertical-margin", settings->verticalMargin));
    argList += (settings->horizontalMargin.empty()) ? "" : (spmenu::addArgument("--horizontal-margin", settings->horizontalMargin));
    argList += spmenu::addArgument("--managed", "--unmanaged", settings->Managed);
    argList += spmenu::addArgument("--wayland", "-x11", settings->displayProtocol);
    argList += spmenu::addArgument("--load-config", "--no-load-config", settings->loadConfig);
    argList += spmenu::addArgument("--load-binds", "--no-load-binds", settings->loadBinds);
    argList += spmenu::addArgument("--load-theme", "--no-load-theme", settings->loadTheme);
    argList += (settings->Font.empty()) ? "" : (spmenu::addArgument("--font", settings->Font));
    argList += (settings->normalItemForegroundColor.empty()) ? "" : (spmenu::addArgument("--normal-item-foreground", settings->normalItemForegroundColor));
    argList += (settings->normalItemBackgroundColor.empty()) ? "" : (spmenu::addArgument("--normal-item-background", settings->normalItemBackgroundColor));
    argList += (settings->selectedItemForegroundColor.empty()) ? "" : (spmenu::addArgument("--selected-item-foreground", settings->selectedItemForegroundColor));
    argList += (settings->selectedItemBackgroundColor.empty()) ? "" : (spmenu::addArgument("--selected-item-background", settings->selectedItemBackgroundColor));
    argList += (settings->normalNextItemForegroundColor.empty()) ? "" : (spmenu::addArgument("--normal-next-item-foreground", settings->normalNextItemForegroundColor));
    argList += (settings->normalNextItemBackgroundColor.empty()) ? "" : (spmenu::addArgument("--normal-next-item-background", settings->normalNextItemBackgroundColor));
    argList += (settings->normalItemPriorityForegroundColor.empty()) ? "" : (spmenu::addArgument("--normal-item-priority-foreground", settings->normalItemPriorityForegroundColor));
    argList += (settings->normalItemPriorityBackgroundColor.empty()) ? "" : (spmenu::addArgument("--normal-item-priority-background", settings->normalItemPriorityBackgroundColor));
    argList += (settings->selectedItemPriorityForegroundColor.empty()) ? "" : (spmenu::addArgument("--selected-item-priority-foreground", settings->selectedItemPriorityForegroundColor));
    argList += (settings->selectedItemPriorityBackgroundColor.empty()) ? "" : (spmenu::addArgument("--selected-item-priority-background", settings->selectedItemPriorityBackgroundColor));
    argList += (settings->promptForegroundColor.empty()) ? "" : (spmenu::addArgument("--prompt-foreground", settings->promptForegroundColor));
    argList += (settings->promptBackgroundColor.empty()) ? "" : (spmenu::addArgument("--prompt-background", settings->promptBackgroundColor));
    argList += (settings->inputForegroundColor.empty()) ? "" : (spmenu::addArgument("--input-foreground", settings->inputForegroundColor));
    argList += (settings->inputBackgroundColor.empty()) ? "" : (spmenu::addArgument("--input-background", settings->inputBackgroundColor));
    argList += (settings->preTextForegroundColor.empty()) ? "" : (spmenu::addArgument("--preText-foreground", settings->preTextForegroundColor));
    argList += (settings->preTextBackgroundColor.empty()) ? "" : (spmenu::addArgument("--preText-background", settings->preTextBackgroundColor));
    argList += (settings->windowColor.empty()) ? "" : (spmenu::addArgument("--menu-background", settings->windowColor));
    argList += (settings->normalHighlightForegroundColor.empty()) ? "" : (spmenu::addArgument("--normal-highlight-foreground", settings->normalHighlightForegroundColor));
    argList += (settings->normalHighlightBackgroundColor.empty()) ? "" : (spmenu::addArgument("--normal-highlight-background", settings->normalHighlightBackgroundColor));
    argList += (settings->selectedHighlightForegroundColor.empty()) ? "" : (spmenu::addArgument("--selected-highlight-foreground", settings->selectedHighlightForegroundColor));
    argList += (settings->selectedHighlightBackgroundColor.empty()) ? "" : (spmenu::addArgument("--selected-highlight-background", settings->selectedHighlightBackgroundColor));
    argList += (settings->numberForegroundColor.empty()) ? "" : (spmenu::addArgument("--number-foreground", settings->numberForegroundColor));
    argList += (settings->numberBackgroundColor.empty()) ? "" : (spmenu::addArgument("--number-background", settings->numberBackgroundColor));
    argList += (settings->modeForegroundColor.empty()) ? "" : (spmenu::addArgument("--mode-foreground", settings->modeForegroundColor));
    argList += (settings->modeBackgroundColor.empty()) ? "" : (spmenu::addArgument("--mode-background", settings->modeBackgroundColor));
    argList += (settings->leftArrowForegroundColor.empty()) ? "" : (spmenu::addArgument("--left-arrow-foreground", settings->leftArrowForegroundColor));
    argList += (settings->leftArrowBackgroundColor.empty()) ? "" : (spmenu::addArgument("--left-arrow-background", settings->leftArrowBackgroundColor));
    argList += (settings->rightArrowForegroundColor.empty()) ? "" : (spmenu::addArgument("--right-arrow-foreground", settings->rightArrowForegroundColor));
    argList += (settings->rightArrowBackgroundColor.empty()) ? "" : (spmenu::addArgument("--right-arrow-background", settings->rightArrowBackgroundColor));
    argList += (settings->caretForegroundColor.empty()) ? "" : (spmenu::addArgument("--caret-foreground", settings->caretForegroundColor));
    argList += (settings->caretBackgroundColor.empty()) ? "" : (spmenu::addArgument("--caret-background", settings->caretBackgroundColor));
    argList += (settings->borderColor.empty()) ? "" : (spmenu::addArgument("--border-background", settings->borderColor));
    argList += (settings->sgr0Color.empty()) ? "" : (spmenu::addArgument("--sgr0", settings->sgr0Color));
    argList += (settings->sgr1Color.empty()) ? "" : (spmenu::addArgument("--sgr1", settings->sgr1Color));
    argList += (settings->sgr2Color.empty()) ? "" : (spmenu::addArgument("--sgr2", settings->sgr2Color));
    argList += (settings->sgr3Color.empty()) ? "" : (spmenu::addArgument("--sgr3", settings->sgr3Color));
    argList += (settings->sgr4Color.empty()) ? "" : (spmenu::addArgument("--sgr4", settings->sgr4Color));
    argList += (settings->sgr5Color.empty()) ? "" : (spmenu::addArgument("--sgr5", settings->sgr5Color));
    argList += (settings->sgr6Color.empty()) ? "" : (spmenu::addArgument("--sgr6", settings->sgr6Color));
    argList += (settings->sgr7Color.empty()) ? "" : (spmenu::addArgument("--sgr7", settings->sgr7Color));
    argList += (settings->sgr8Color.empty()) ? "" : (spmenu::addArgument("--sgr8", settings->sgr8Color));
    argList += (settings->sgr9Color.empty()) ? "" : (spmenu::addArgument("--sgr9", settings->sgr9Color));
    argList += (settings->sgr10Color.empty()) ? "" : (spmenu::addArgument("--sgr10", settings->sgr10Color));
    argList += (settings->sgr11Color.empty()) ? "" : (spmenu::addArgument("--sgr11", settings->sgr11Color));
    argList += (settings->sgr12Color.empty()) ? "" : (spmenu::addArgument("--sgr12", settings->sgr12Color));
    argList += (settings->sgr13Color.empty()) ? "" : (spmenu::addArgument("--sgr13", settings->sgr13Color));
    argList += (settings->sgr14Color.empty()) ? "" : (spmenu::addArgument("--sgr14", settings->sgr14Color));
    argList += (settings->sgr15Color.empty()) ? "" : (spmenu::addArgument("--sgr15", settings->sgr15Color));

    if (!settings->windowPosition.empty()) {
        std::string argument = "";
        if (!settings->windowPosition.compare("Top")) {
            argument = "--top";
        } else if (!settings->windowPosition.compare("Bottom")) {
            argument = "--bottom";
        } else if (!settings->windowPosition.compare("Center")) {
            argument = "--center";
        }

        argList += " " + argument;
    }

    if (!settings->itemPosition.empty()) {
        std::string argument = "";
        if (!settings->itemPosition.compare("Top")) {
            argument = "--item-top";
        } else if (!settings->itemPosition.compare("Bottom")) {
            argument = "--item-bottom";
        }

        argList += " " + argument;
    }

    if (!settings->imagePosition.empty()) {
        std::string argument = "";
        if (!settings->imagePosition.compare("Top")) {
            argument = "--image-top";
        } else if (!settings->imagePosition.compare("Bottom")) {
            argument = "--image-bottom";
        } else if (!settings->imagePosition.compare("Center")) {
            argument = "--image-center";
        } else if (!settings->imagePosition.compare("Top Center")) {
            argument = "--image-topcenter";
        }

        argList += " " + argument;
    }

    return argList;
}

std::string spmenu::Run(spmenu::spmenuSettings *settings) {
    const int bufferSize{ sizeof(settings->standardInput) };
    std::ostringstream command;
    std::string ret;
    std::array<char, bufferSize> buf;

    std::string Arguments;

    Arguments = spmenu::generateArguments(settings);

    command << "printf \"" << settings->standardInput << "\" | spmenu " << Arguments << " " << settings->userArguments;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.str().c_str(), "r"), pclose);

    if (!pipe) {
        std::cerr << "Failed to popen()\n";
        return ret;
    }

    while (fgets(buf.data(), buf.size(), pipe.get()) != NULL) {
        ret += buf.data();
    }

    return ret;
}
