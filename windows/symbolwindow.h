#pragma once

class SymbolStore;

class SymbolWindow
{
public:
	SymbolWindow();
	virtual ~SymbolWindow();
	
	void Draw(bool* pShow);
	void SetSymbolStore(SymbolStore* p);
private:
	SymbolStore* pStore;
};
