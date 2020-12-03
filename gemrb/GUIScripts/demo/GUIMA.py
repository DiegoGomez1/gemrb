# -*-python-*-
# GemRB - Infinity Engine Emulator
# Copyright (C) 2020 The GemRB Project
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#

# GUIMA.py - scripts to control map windows from the GUIMA and GUIWMAP winpacks

###################################################

import GemRB
import GUICommonWindows
import GUIMACommon
from GUIDefines import *

WorldMapControl = None
AreaMapControl = None

def InitMapWindow (Window, WorldMap = False, Travel = -1):
	global WorldMapControl, AreaMapControl

	Label = Window.GetControl (0)
	if WorldMap:
		Label.SetText ("World map")
	else:
		Label.SetText ("Area map")

	# World Map
	Button = Window.GetControl (1)
	if WorldMap:
		Button.SetText ("MAP")
	else:
		Button.SetText ("WMAP")
	Button.SetEvent (IE_GUI_BUTTON_ON_PRESS, lambda: InitMapWindow (Window, not WorldMap, Travel))

	# Map or World Map control
	if WorldMap:
		if AreaMapControl:
			AreaMapControl.SetVisible (False)
			AreaMapControl.SetDisabled (True)

		if WorldMapControl:
			WorldMapControl.SetVisible (True)
			WorldMapControl.SetDisabled (False)
		else:
			WorldMapControl = Window.ReplaceSubview (2, IE_GUI_WORLDMAP, Travel, "floattxt")
			WorldMapControl.SetAnimation ("WMDAG")
			WorldMapControl.SetEvent (IE_GUI_WORLDMAP_ON_PRESS, GUIMACommon.MoveToNewArea)
			WorldMapControl.SetAction (ChangeTooltip, IE_ACT_MOUSE_ENTER)

		# center on current area
		WorldMapControl.Scroll (0, 0, False)
		WorldMapControl.Focus ()
	else:
		if WorldMapControl:
			WorldMapControl.SetVisible (False)
			WorldMapControl.SetDisabled (True)

		if AreaMapControl:
			AreaMapControl.SetVisible (True)
			AreaMapControl.SetDisabled (False)
		else:
			AreaMapControl = Window.ReplaceSubview (4, IE_GUI_MAP)
			AreaMapControl.SetAction (CloseMapWindow, IE_ACT_MOUSE_PRESS, GEM_MB_ACTION, 0, 2)
		AreaMapControl.Focus ()

	Button = Window.GetControl (3)
	Button.SetText ("Close")
	Button.SetEvent (IE_GUI_BUTTON_ON_PRESS, CloseMapWindow)
	Button.SetHotKey ('m')

	# workaround for proper closure with ESC
	Button = Window.GetControl (99)
	if not Button:
		Button = Window.CreateButton (99, 0, 0, 0, 0)
	Button.SetEvent (IE_GUI_BUTTON_ON_PRESS, CloseMapWindow)
	Button.MakeEscape ()

	return

def CloseMapWindow ():
	global WorldMapControl, AreaMapControl

	WorldMapControl = None
	AreaMapControl = None
	GUICommonWindows.CloseTopWindow ()

ToggleMapWindow = GUICommonWindows.CreateTopWinLoader (0, "GUIMAP", GUICommonWindows.ToggleWindow, InitMapWindow)
OpenMapWindow = GUICommonWindows.CreateTopWinLoader (0, "GUIMAP", GUICommonWindows.OpenWindowOnce, InitMapWindow)

def OpenTravelWindow ():
	Window = OpenMapWindow ()
	InitMapWindow (Window, True, GemRB.GetVar ("Travel"))
	return

def ChangeTooltip ():
	global WorldMapControl

	tt = ""
	area = WorldMapControl.GetDestinationArea ()
	if area and area["Distance"] >= 0:
		str = GemRB.GetString (4)
		if (str):
			tt = "%s: %d"%(str,area["Distance"])

	WorldMapControl.SetTooltip (tt)
	return
