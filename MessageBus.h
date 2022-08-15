#pragma once
#include <iostream>

class MessageBus
{
private:
	float nr;

private:
	MessageBus();

	static MessageBus& GetInstance() 
	{ 
		static MessageBus instance; 
		return instance;
	}

public:
	~MessageBus();
	MessageBus& operator=(const MessageBus& other) = delete;
	MessageBus(const MessageBus& other) = delete;
};