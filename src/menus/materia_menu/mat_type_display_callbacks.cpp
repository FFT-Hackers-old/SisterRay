#include "mat_type_display_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"

using namespace MateriaWidgetNames;

void updateEquipEffect(Widget* equipEffectWidget, MateriaInventoryEntry materia) {
    auto equipEffect = gContext.materias.getResource(materia.materiaID).gameMateria.equipEffect;
    return;
}

void ablListH0S2(const DrawMateriaDataEvent* event) {
    auto abilityListWidget = getChild(event->displayWidget, ABILITIES_LIST);
    auto& data = gContext.materias.getResource(event->materia.materiaID).gameMateria.data;
    enableWidget(abilityListWidget);
    EncodedString statName;
    if (data[0] <= 10)
        statName = gContext.gameStrings.statNames.getResource(data[0]);
    else {
        statName = gContext.gameStrings.materiaTexts.getResource(data[0]);
    }
    for (auto idx = 0; idx < 5; ++idx) {
        auto rowWidget = getChild(abilityListWidget, idx);
        flagPercent(rowWidget, true);
        if (data[idx + 1] == 0xFF) {
            disableWidget(rowWidget);
            continue;
        }
        enableWidget(rowWidget);
        updateText(getChild(rowWidget, std::string("TXT")), statName.str());
        updateNumber(getChild(rowWidget, std::string("AMT")), data[idx + 1]);
        if (idx <= event->materiaLevel - 1) {
            updateTextColor(getChild(rowWidget, std::string("TXT")), COLOR_WHITE);
            updateNumberColor(getChild(rowWidget, std::string("AMT")), COLOR_WHITE);
            continue;
        }
        updateTextColor(getChild(rowWidget, std::string("TXT")), COLOR_GRAY);
    }
}

void ablListH9S1(const DrawMateriaDataEvent* event){
    auto abilityListWidget = getChild(event->displayWidget, ABILITIES_LIST);
    auto& data = gContext.materias.getResource(event->materia.materiaID).gameMateria.data;
    enableWidget(abilityListWidget);
    for (auto idx = 0; idx < 5; ++idx) {
        auto widget = getChild(abilityListWidget, idx);
        flagPercent(widget, false);
        if (data[idx] == 0xFF) {
            disableWidget(widget);
            continue;
        }
        enableWidget(widget);
        auto magicIndex = data[idx];
        auto& abilityName = getCommandAction(CMD_MAGIC, magicIndex).name;
        updateText(getChild(widget, std::string("TXT")), abilityName.str());
        if (idx <= event->materiaLevel - 1) {
            updateTextColor(getChild(widget, std::string("TXT")), COLOR_WHITE);
            continue;
        }
        updateTextColor(getChild(widget, std::string("TXT")), COLOR_GRAY);
    }
}


void ablListH11S3(const DrawMateriaDataEvent* event) {
    auto abilityListWidget = getChild(event->displayWidget, ABILITIES_LIST);
    auto& data = gContext.materias.getResource(event->materia.materiaID).gameMateria.data;
    enableWidget(abilityListWidget);
    for (auto idx = 0; idx < 5; ++idx) {
        for (auto idx = 0; idx < 5; ++idx) {
            auto widget = getChild(abilityListWidget, idx);
            flagPercent(widget, false);
            if (idx == 0) {
                if (data[0] == 0xFF) {
                    disableWidget(widget);
                    break;
                }
                enableWidget(widget);
                auto summonIndex = data[0];
                auto& abilityName = getCommandAction(CMD_SUMMON, summonIndex).name;
                updateText(getChild(widget, std::string("TXT")), abilityName.str());
                updateTextColor(widget, COLOR_WHITE);
                continue;
            }
            disableWidget(widget);
        }
    }
}

void flagPercent(Widget* widget, bool isEnabled) {
    if (isEnabled) {
        enableWidget(getChild(widget, "AMT"));
        enableWidget(getChild(widget, "SIGN"));
        enableWidget(getChild(widget, "PCNT"));
        return;
    }
    disableWidget(getChild(widget, "AMT"));
    disableWidget(getChild(widget, "SIGN"));
    disableWidget(getChild(widget, "PCNT"));
}
