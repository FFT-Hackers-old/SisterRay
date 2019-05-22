#include "mat_type_display_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"

using namespace MateriaWidgetNames;

void updateEquipEffect(Widget* equipEffectWidget, MateriaInventoryEntry materia) {
    auto equipEffect = gContext.materias.get_resource(materia.item_id).equipEffect;
    return;
}

void ablListH0S2(const DrawMateriaDataEvent* event) {
    auto abilityListWidget = getChild(event->displayWidget, ABILITIES_LIST);
    auto& data = gContext.materias.get_resource(event->materia.item_id).data;
    enableWidget(abilityListWidget);
    EncodedString statName;
    if (data[0] <= 10)
        statName = gContext.gameStrings.statNames.get_resource(data[0]);
    else {
        statName = gContext.gameStrings.materiaTexts.get_resource(data[0]);
    }
    for (auto idx = 0; idx < 5; ++idx) {
        auto widget = getChild(abilityListWidget, idx);
        flagPercent(widget, true);
        if (data[idx + 1] == 0xFF) {
            disableWidget(widget);
            continue;
        }
        enableWidget(widget);
        updateText(getChild(widget, std::string("TXT")), statName.str());
        updateNumber(getChild(widget, std::string("AMT")), data[idx + 1]);
        if (idx <= event->materiaLevel - 1) {
            updateTextColor(getChild(widget, std::string("TXT")), COLOR_WHITE);
            continue;
        }
        updateTextColor(getChild(widget, std::string("TXT")), COLOR_GRAY);
    }
}

void ablListH9S1(const DrawMateriaDataEvent* event){
    auto abilityListWidget = getChild(event->displayWidget, ABILITIES_LIST);
    auto& data = gContext.materias.get_resource(event->materia.item_id).data;
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
        auto& abilityName = gContext.attacks.get_element(std::string("MAG") + std::to_string(magicIndex)).attackName;
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
    auto& data = gContext.materias.get_resource(event->materia.item_id).data;
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
                auto& abilityName = gContext.attacks.get_element(std::string("SUM") + std::to_string(summonIndex)).attackName;
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
