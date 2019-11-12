#pragma once

class SymbolStore;

class SymbolWindow
{
public:
	SymbolWindow();
	virtual ~SymbolWindow();
	
	void Draw();
	void SetSymbolStore(SymbolStore* p);
private:
	SymbolStore* pStore;
};
