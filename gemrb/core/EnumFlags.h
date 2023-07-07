/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2021 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 */

#ifndef EnumFlags_h
#define EnumFlags_h

#include <type_traits>

template <typename ENUM>
using under_t = std::underlying_type_t<ENUM>;

template<typename ENUM_FLAGS>
constexpr
under_t<ENUM_FLAGS>
UnderType(ENUM_FLAGS a) noexcept
{
	return static_cast<under_t<ENUM_FLAGS>>(a);
}

template<typename ENUM_FLAGS>
constexpr
std::enable_if_t<std::is_enum<ENUM_FLAGS>::value, ENUM_FLAGS>
operator |(ENUM_FLAGS a, ENUM_FLAGS b) noexcept
{
	return static_cast<ENUM_FLAGS>(UnderType(a) | UnderType(b));
}

template<typename ENUM_FLAGS>
constexpr
std::enable_if_t<std::is_enum<ENUM_FLAGS>::value, ENUM_FLAGS&>
operator |=(ENUM_FLAGS& a, ENUM_FLAGS b) noexcept
{
	return a = a | b;
}

template<typename ENUM_FLAGS>
constexpr
std::enable_if_t<std::is_enum<ENUM_FLAGS>::value, ENUM_FLAGS>
operator &(ENUM_FLAGS a, ENUM_FLAGS b) noexcept
{
	return static_cast<ENUM_FLAGS>(UnderType(a) & UnderType(b));
}

template<typename ENUM_FLAGS>
constexpr
std::enable_if_t<std::is_enum<ENUM_FLAGS>::value, ENUM_FLAGS&>
operator &=(ENUM_FLAGS& a, ENUM_FLAGS b) noexcept
{
	return a = a & b;
}

template<typename ENUM_FLAGS>
constexpr
std::enable_if_t<std::is_enum<ENUM_FLAGS>::value, ENUM_FLAGS>
operator ^(ENUM_FLAGS a, ENUM_FLAGS b) noexcept
{
	return static_cast<ENUM_FLAGS>(UnderType(a) ^ UnderType(b));
}

template<typename ENUM_FLAGS>
constexpr
std::enable_if_t<std::is_enum<ENUM_FLAGS>::value, ENUM_FLAGS&>
operator ^=(ENUM_FLAGS& a, ENUM_FLAGS b) noexcept
{
	return a = a ^ b;
}

template<typename ENUM_FLAGS>
constexpr
std::enable_if_t<std::is_enum<ENUM_FLAGS>::value, ENUM_FLAGS>
operator ~(ENUM_FLAGS a) noexcept
{
	return static_cast<ENUM_FLAGS>(~UnderType(a));
}

template<typename ENUM_FLAGS>
constexpr
std::enable_if_t<std::is_enum<ENUM_FLAGS>::value, bool>
operator !(ENUM_FLAGS a) noexcept
{
	return !UnderType(a);
}

#endif /* EnumFlags_h */
