/******************************************************************************************
*	Chili Direct3D Engine																  *
*	Copyright 2018 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of Chili Direct3D Engine.											  *
*																						  *
*	Chili Direct3D Engine is free software: you can redistribute it and/or modify		  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili Direct3D Engine is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili Direct3D Engine.  If not, see <http://www.gnu.org/licenses/>.    *
******************************************************************************************/
#pragma once
#include "ChiliWin.h"
#include <exception>
#include <string>

#define CHWND_EXCEPT(hr) HrException(__LINE__, __FILE__, hr)
#define CHWND_LAST_EXCEPT() HrException(__LINE__, __FILE__, GetLastError())

class ChiliException : public std::exception
{
public:
	ChiliException( int line,const char* file ) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	int mLine;
	std::string mFile;
protected:
	mutable std::string whatBuffer;
};

class Exception : public ChiliException
{
	using ChiliException::ChiliException;
public:
	static std::string TranslateErrorCode(HRESULT hr) noexcept;
};

class HrException : public Exception
{
public:
	HrException(int line, const char* file, HRESULT hr) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorDescription() const noexcept;
private:
	HRESULT hr;
};

class NoGfxException : public Exception
{
public:
	using Exception::Exception;
	const char* GetType() const noexcept override;
};