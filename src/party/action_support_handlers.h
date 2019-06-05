#ifndef ACTION_SUPPORT_HANDLERS_H
#define ACTION_ENABLED_HANDLERS_H

#include <SisterRay/SisterRay.h>
#include "../events/party_events.h"

void counterT6S1(const ApplySupportEvent* event);
void mCounterT9S1(const ApplySupportEvent* event);
void mCounterT11S3(const ApplySupportEvent* event);
void sAttackT9S1(const ApplySupportEvent* event);
void sAttackT11S3(const ApplySupportEvent* event);
void fAttackT9S1(const ApplySupportEvent* event);
void fAttackT11S3(const ApplySupportEvent* event);

#endif
