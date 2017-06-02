#ifndef MESSENGER_H_INCLUDED
#define MESSENGER_H_INCLUDED

#include "MessageRegistration.h"

class Messenger : public MessageRegistrator
{
private:
	String logfile;
	bool noverbose;
public:
	bool has_error;
	Messenger();
	virtual void AddMessage(const String& message, const MessageState mstate, TStringList* param = NULL);
	virtual void Status(const String& message);
	void setlogfile(String _logfile);
	void setnoverbose(bool nv){noverbose = nv;};
};

#endif // MESSENGER_H_INCLUDED
