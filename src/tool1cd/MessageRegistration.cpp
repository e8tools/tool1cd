//---------------------------------------------------------------------------

#include "MessageRegistration.h"

using namespace std;

MessageDetails MessageRegistrator::AddMessage(const string &description, const MessageState mstate)
{
	MessageDetails details(*this, static_cast<string>(description), mstate);
	return details;
}

MessageDetails MessageRegistrator::AddError(const string &description)
{
	return AddMessage(description, MessageState::Error);
}

MessageDetails MessageRegistrator::AddDebugMessage(const string &description, const MessageState mstate)
{
	// TODO: AddDebugMessage
	return AddMessage(description, mstate);
}

MessageRegistrator::MessageRegistrator()
{

}

MessageRegistrator::~MessageRegistrator()
{

}

void MessageRegistrator::setDebugMode(bool dstate)
{
	DebugMessage = dstate;
}

bool MessageRegistrator::getDebugMode()
{
	return DebugMessage;
}


void Registrator::AddMessageRegistrator(MessageRegistrator* messageregistrator)
{
	msreg_m = messageregistrator;
}

void Registrator::RemoveMessageRegistrator()
{
	msreg_m = nullptr;
}

void Registrator::Status(const string &message)
{

}

void Registrator::AddDetailedMessage(const string &description, const MessageState mstate,
									 const TStringList *param)
{
	if (msreg_m) {
		msreg_m->AddDetailedMessage(description, mstate, param);
	}
}