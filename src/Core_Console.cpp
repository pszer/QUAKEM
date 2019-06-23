#include "Core.hpp"

void Core::ConsoleUpdate() {
	if (Console.open)
		Event.EnableTextInput();
	else
		Event.DisableTextInput();
}

void Core::ConsoleRender() {
	if (!Console.open) return;

	Font * font_struct = Media.GetFont(Console.font);
	if (font_struct == nullptr || font_struct->type != FONT_TTF) return;
	TTF_Font * font = font_struct->GetTTFSize(FONT_P16);

	Rect bg_rect = { 0 , 0 , Event.win_w , Event.win_h - Console.bottom_offset };
	SDL_Rect rect = { 5, Event.win_h - Console.bottom_offset, 0, 0 };
	TTF_SizeText(font, " ", &rect.w, &rect.h);
	rect.y -= rect.h*2 + 5;

	// background
	Renderer.RenderFillRect(bg_rect, Console.bg);

	// render text
	int i = Log::History.size()-1;
	while (rect.y >= 0 && i >= 0) {
		const std::string& str = Log::History.at(i);

		TTF_SizeText(font, str.c_str(), &rect.w, &rect.h);

		Renderer.RenderText(font, str, rect.x, rect.y, Console.fg);

		rect.y -= (rect.h+1);
		--i;
	}

	int width;
	// render text being typed
	TTF_SizeText(font, Console.text.c_str(), &rect.w, &rect.h);
	TTF_SizeText(font, "H", &width, nullptr);
	rect.x = 2;
	rect.y = Event.win_h - Console.bottom_offset - rect.h - 2;
	Renderer.RenderText(font, Console.text, rect.x, rect.y, Console.fg);

	rect.x += (Console.cursor) * width + 1;
	rect.w = 2;
	Renderer.RenderFillRect(Rect(rect.x,rect.y,rect.w,rect.h), Console.fg);
}

void Core::Console::Toggle() {
	if (open)
		Event.DisableTextInput();
	else
		Event.EnableTextInput();
	open = !open;
}

void Core::ConsoleEnter() {
	if (!Console.open) return;
	Log::Add(Console.text);
	Command(Console.text);

	Console.History.insert(Console.History.begin(), Console.text);
	if (Console.History.size() > Console.HISTORY_MAX)
		Console.History.pop_back();
	Console.history_scroll = 0;

	Console.Reset();
}

void Core::Console::Reset() {
	text = "";
	cursor = 0;
}

void Core::Console::Backspace() {
	if (!open) return;
	if (!text.empty()) {
		int _cursor = cursor-1;
		if (cursor == 0) _cursor = 0;
		text.erase(text.begin() + _cursor);
		Left();
	}
}

void Core::Console::TextInput(const std::string& input) {
	if (!open) return;
	if (input == "`") return;
	text = text.insert(cursor, input);
	cursor += input.length();
}

void Core::Console::Left() {
	if (!open) return;
	cursor--;
	if (cursor < 0) cursor = 0;
}

void Core::Console::Right() {
	if (!open) return;
	cursor++;
	if (cursor > text.length()) cursor = text.length();
}

void Core::Console::Up() {
	if (!open) return;
	history_scroll++;
	if (history_scroll > History.size())
		history_scroll = History.size();
	SetHistoryText();
}

void Core::Console::Down() {
	if (!open) return;
	history_scroll--;
	if (history_scroll < 0) history_scroll = 0;
	SetHistoryText();
}

void Core::Console::SetHistoryText() {
	if (history_scroll == 0)
		text = "";
	else
		text = History.at(history_scroll-1);
	cursor = std::min((int)cursor, (int)text.length());
}
