#include <string.h>
#include "EEPROM.h"

#include "settings.h"
#include "settings_eeprom.h"
#include "timer.h"
#ifdef ENABLE_UI
#include "ui.h"
#endif

static Timer saveTimer = Timer(EEPROM_SAVE_TIME);
static bool isDirty = false;

struct EepromSettings EepromSettings;

void EepromSettings::setup() {
    this->load();
}
#ifdef ENABLE_UI
void EepromSettings::update() {
    if (isDirty && saveTimer.hasTicked() && !Ui::isTvOn) {
        isDirty = false;
        saveTimer.reset();
        this->save();
    }
}
#else
void EepromSettings::update() {
    if (isDirty && saveTimer.hasTicked()) {
        isDirty = false;
        saveTimer.reset();
        this->save();
    }
}
#endif
void EepromSettings::load() {
    EEPROM.get(0, *this);
    
    if (this->versionNumber != VERSION_NUMBER)
        this->initDefaults();
}

void EepromSettings::save() {
    EEPROM.put(0, *this);
    EEPROM.commit();
}

void EepromSettings::markDirty() {
    isDirty = true;
}

void EepromSettings::initDefaults() {
    memcpy_P(this, &EepromDefaults, sizeof(EepromDefaults));
    this->save();
}
