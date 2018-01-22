#ifndef MESSENGER_H_INCLUDED
#define MESSENGER_H_INCLUDED

#include "MessageRegistration.h"

class Messenger : public MessageRegistrator
{
private:
	String logfile;
	bool noverbose;
	MessageState minimal_state;
public:
	bool has_error;
	Messenger();
	virtual void AddMessage(const String &message, const MessageState mstate, const TStringList *param = nullptr) override;
	virtual void Status(const String& message) override;
	void setlogfile(String _logfile);
	void setnoverbose(bool nv){noverbose = nv;}
	void set_log_level(const MessageState _minimal_state);
};

#endif // MESSENGER_H_INCLUDED
