/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2003-2004 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 */

#include "Audio.h"

namespace GemRB {

const TypeID Audio::ID = { "Audio" };

void Audio::UpdateChannel(const std::string& name, int idx, int volume, float reverb)
{
	channels[idx] = Channel(name);

	volume = Clamp(volume, 0, 100);
	channels[idx].setVolume(volume);

	reverb = Clamp(reverb, 0.0f, 100.0f);
	channels[idx].setReverb(reverb);
}

SFXChannel Audio::GetChannel(const std::string& name) const
{
	for (SFXChannel channelID : EnumIterator<SFXChannel>()) {
		if (channels[channelID].getName() == name) {
			return channelID;
		}
	}
	return SFXChannel::count;
}

int Audio::GetVolume(SFXChannel channel) const
{
	if (channel >= SFXChannel::count) {
		return 100;
	}
	return channels[channel].getVolume();
}

float Audio::GetReverb(SFXChannel channel) const
{
	if (channel >= SFXChannel::count) {
		return 0.0f;
	}
	return channels[channel].getReverb();
}

float Audio::GetHeight(SFXChannel channel) const
{
	if (channel >= SFXChannel::count) {
		return 0.0f;
	}
	return channels[channel].getHeight(channel);
}

Holder<SoundHandle> Audio::PlayMB(const String& resource, SFXChannel channel, const Point& p, unsigned int flags, tick_t* length)
{
	auto mbString = MBStringFromString(resource);
	auto mbResource = StringView{mbString};

	return Play(mbResource, channel, p, flags, length);
}

}
