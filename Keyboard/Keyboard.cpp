#include "Keyboard.h"

Keyboard::Keyboard()
{
}

bool Keyboard::KeyIsPressed(const unsigned char keycode)
{
	return keyStates[keycode];
}

bool Keyboard::KeyBufferIsEmpty() const
{
	return keyBuffer.empty();
}

bool Keyboard::CharBufferIsEmpty() const
{
	return charBuffer.empty();
}

KeyboardEvent Keyboard::ReadKey()
{
	if (keyBuffer.empty())
	{
		return KeyboardEvent();
	}

	KeyboardEvent e = keyBuffer.front();
	keyBuffer.pop();
	return e;
}

unsigned char Keyboard::ReadChar()
{
	if (charBuffer.empty())
	{
		return 0u;
	}

	unsigned char e = charBuffer.front();
	charBuffer.pop();

	return e;
}

void Keyboard::OnKeyPressed(const unsigned char key)
{
	keyStates[key] = true;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Pressed, key));
}

void Keyboard::OnKeyReleased(const unsigned char key)
{
	keyStates[key] = false;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Released, key));
}

void Keyboard::OnChar(const unsigned char key)
{
	charBuffer.push(key);
}

void Keyboard::EnableAutoRepeatKeys()
{
	autoRepeatKeys = true;
}

void Keyboard::DisableAutoRepeatKeys()
{
	autoRepeatKeys = false;
}

void Keyboard::EnableAutoRepeatChars()
{
	autoRepeatChars = true;
}

void Keyboard::DisableAutoRepeatChars()
{
	autoRepeatChars = false;
}

bool Keyboard::IsKeysAutoRepeat()
{
	return autoRepeatKeys;
}

bool Keyboard::IsCharsAutoRepeat()
{
	return autoRepeatChars;
}
