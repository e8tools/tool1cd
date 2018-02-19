
#ifndef MessageRegistrationH
#define MessageRegistrationH

#include <System.Classes.hpp>

//---------------------------------------------------------------------------
enum class MessageState
{
	Empty = -1,
	Hint = 0,
	Succesfull = 1,
	Info = 2,
	Warning = 3,
	Error = 4
};

class IMessageRegistration
{
public:
	virtual void AddDetailedMessage(const String &description, const MessageState mstate, const TStringList *param) = 0;
	virtual void Status(const String& message) = 0;
};

class MessageDetails
{
public:
	MessageDetails(IMessageRegistration &owner, const std::string &initial_message, MessageState mstate)
			: owner(owner), initial_message(initial_message), mstate(mstate)
	{}

	MessageDetails &with(const std::string &param_name, const std::string &value)
	{
		std::string total_string(param_name);
		total_string.append("=").append(value);
		data.push_back(total_string);
		return *this;
	}

	MessageDetails &with(const std::string &param_name, const String &value)
	{
		return with(param_name, static_cast<std::string>(value));
	}

	template <typename value_type>
	MessageDetails &with(const std::string &param_name, value_type value)
	{
		return with(param_name, std::to_string(value));
	}

	~MessageDetails()
	{
		owner.AddDetailedMessage(initial_message, mstate, &data);
	}
private:
	IMessageRegistration &owner;
	std::string initial_message;
	MessageState mstate;
	TStringList data;
};

//---------------------------------------------------------------------------
class MessageRegistrator: public IMessageRegistration
{
private:
	bool DebugMessage;

public:
	MessageRegistrator();
	virtual ~MessageRegistrator();
	void setDebugMode(bool dstate);
	bool getDebugMode();

	virtual MessageDetails AddMessage(const String &description, const MessageState mstate);
	virtual MessageDetails AddError(const String& description);
	virtual MessageDetails AddDebugMessage(const String& description, const MessageState mstate);

};

//---------------------------------------------------------------------------

class IControlMessageRegistration
{
public:
	virtual ~IControlMessageRegistration() {}
	virtual void AddMessageRegistrator(MessageRegistrator* messageregistrator) = 0;
	virtual void RemoveMessageRegistrator() = 0;
};

class Registrator: public MessageRegistrator, public IControlMessageRegistration
{
public:
	void AddDetailedMessage(const String& description, const MessageState mstate,
					const TStringList* param) override;
	void Status(const String& message) override;

	void AddMessageRegistrator(MessageRegistrator* messageregistrator);
	void RemoveMessageRegistrator();
private:
	MessageRegistrator* msreg_m{nullptr};
};

#endif

