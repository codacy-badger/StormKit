#pragma once

#include <storm/window/Key.hpp>
#include <storm/window/MouseButton.hpp>
#include <storm/window/VideoSettings.hpp>

#import <Foundation/NSGeometry.h>

storm::window::MouseButton toStormMouseButton(int button);
storm::window::Key localizedKeyToStormKey(char code);
storm::window::Key nonLocalizedKeytoStormKey(unsigned short code);
storm::window::VideoSettings::Size toStormVec(NSPoint point);
storm::window::VideoSettings::Size toStormVec(CGSize size);
NSPoint fromStormVec(const storm::window::VideoSettings::Size &point);
