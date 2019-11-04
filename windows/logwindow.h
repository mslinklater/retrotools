#pragma once

class LogWindow
{
public:
	LogWindow();
	virtual ~LogWindow();
	
	void Draw();
private:
	bool showInfo;
	bool showWarnings;
	bool showErrors;
};
