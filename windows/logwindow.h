#pragma once

class LogWindow
{
public:
	LogWindow();
	virtual ~LogWindow();
	
	void Draw(bool* pOpen);
private:
	bool showInfo;
	bool showWarnings;
	bool showErrors;
};
