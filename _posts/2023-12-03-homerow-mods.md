---
layout: post
title: Homerow Mods in OS X
summary: Tutorial to configure homerow mods in OS X with KMonad for the internal keyboard
date: 2023-12-03 12:00
category: tools
tags: [osx, kmonad, keyboard]
---
{% include JB/setup %}

Since I got the Advantage 360 keyboard I've been experimenting with keyboard
customizations, and I've found homerow mods to be a very comfortable way to
use shortcuts at scale without having to move your hands from the home row. The
idea is use the `asdf` and `jkl;` keys as modifiers when pressed in combination 
with other keys but as regular keys when pressed alone ([longer explanation
here](precondition_homerow_mods)). Once you get used to it, you feel you have 
one hand tied to your back when you use a regular keyboard.

Unfortunately, I cannot customize the integrated keyboard of my MacBook laptop
using [ZMK](zmk) or [QMK](qmk), so I had to find another way. I found
[KMonad][kmonad], a program that intercepts your keyboard taps and modifies
them according to a configuration file written in pseudo-lisp.

## Setup procedure
1. Install Stack. Stack is a (the?) Haskell build tool. KMonad is written in 
   Haskell as you can guess from such a name. You can install it using `brew 
   install haskell-stack`.
2. KMonad piggybacks on the kernel extension that 
   [Karabiner-Elements][karabiner] uses to intercept keyboard events. Install
   the [Karabiner driver][driver] or the full thing.
3. Activate the driver:
   ```shell
   /Applications/.Karabiner-VirtualHIDDevice-Manager.app/Contents/MacOS/Karabiner-VirtualHIDDevice-Manager activate
   ```
4. Clone the KMonad repo and build it:
   ```shell
   git clone --recursive https://github.com/vosaica/kmonad.git
   cd kmonad
   git checkout Upgrade-DriverKit-VirtualHIDDevice
   git submodule update --init
   stack build --flag kmonad:dext --extra-include-dirs=c_src/mac/Karabiner-DriverKit-VirtualHIDDevice/include/pqrs/karabiner/driverkit:c_src/mac/Karabiner-DriverKit-VirtualHIDDevice/src/Client/vendor/include
   ```
5. Create a configuration file at `~/.kmonad.kbd`. You can start from mine and
   modify it to your liking after taking a look at the 
   [KMonad tutorial][kmonad-tutorial]. I'm using Command-Alt-Shift-Ctrl as my
   mapping for the left hand (and reversed for the right one) but other people
   use different combinations. See the [Precondition post][precondition_homerow_mods]
   for more details.
   ```
   (defcfg
       ;; Limit KMonad to the integrated keybboard, use (iokit-name) to apply to all
       input  (iokit-name "Apple Internal Keyboard / Trackpad")
       output (kext)
       
       ;; Comment this if you want unhandled events not to be emitted
       fallthrough true
       
       ;; Set this to false to disable any command-execution in KMonad
       allow-cmd true
   )
   
   ;; lmet -> cmd
   ;; lalt -> option
   ;; fn   -> fn

   (defsrc
       esc  f1   f2   f3   f4   f5   f6   f7   f8   f9   f10  f11  f12
       grv  1    2    3    4    5    6    7    8    9    0    -    =    bspc
       tab  q    w    e    r    t    y    u    i    o    p    [    ]    \
       caps a    s    d    f    g    h    j    k    l    ;    '    ret
       lsft z    x    c    v    b    n    m    ,    .    /    rsft up
       fn   lctl lalt lmet           spc            rmet ralt left down rght
   )
   
   (defalias
       met_a (tap-hold-next-release 200 a lmet)
       alt_s (tap-hold-next-release 200 s lalt)
       sft_d (tap-hold-next-release 200 d lsft)
       ctl_f (tap-hold-next-release 200 f lctl)
   
       ctl_j (tap-hold-next-release 200 j rctl)
       sft_k (tap-hold-next-release 200 k rsft)
       alt_l (tap-hold-next-release 200 l lalt)
       met_; (tap-hold-next-release 200 ; rmet)
   
       ;; let's use caps as control unless double-tapped
       ccaps (multi-tap 300 lctl caps)
   
       fn (around (layer-toggle function) fn)
   )
   
   (deflayer default
     _       f1      f2      f3      f4      f5      f6      f7      f8      f9      f10     f11     f12
     _       _       _       _       _       _       _       _       _       _       _       _       _    _
     _       _       _       _       _       _       _       _       _       _       _       _       _    _
     @ccaps  @met_a  @alt_s  @sft_d  @ctl_f  _       _       @ctl_j  @sft_k  @alt_l  @met_;  _       _
     _       _       _       _       _       _       _       _       _       _       _       _       _
     @fn     _       _       _                       _                       _       _       _       _    _
   )
   
   ;; We need to explicitly map the function keys to their original function because
   ;; KMonad precludes the default configuration. As a bonus, enable vim-like cursor when pressing fn.
   (deflayer function
     _       brdn    brup    lp      mctl    bldn    blup    prev    pp      next    mute    vold    volu
     _       _       _       _       _       _       _       _       _       _       _       _       _    _
     _       _       _       _       _       _       _       _       _       _       _       _       _    _
     _       _       _       _       _       _       left    down    up      right   _       _       _
     _       _       _       _       _       _       _       _       _       _       _       _       _
     _       _       _       _                       _                       _       _       _       _    _
   )
   ```
6. Go to Settings > Privacy & Security > Input Monitoring and add 
   `/bin/launchctl` and the kmonad binary.
7. Test the configuration:
   ```shell
   sudo kmonad ~/.kmonad.kbd
   ```
8. If everything works as expected, you can install KMonad as a service by creating
   a launch config at `/Library/LaunchDaemons/local.kmonad.plist` (adjust paths if
   you installed kmonad elsewhere):
   ```xml
   <?xml version="1.0" encoding="utf-8"?>
   <!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
   <plist version="1.0">
     <dict>
       <key>Label</key>
       <string>local.kmonad</string>
       <key>Program</key>
       <string>/Users/your_username/.local/bin/kmonad</string>
       <key>ProgramArguments</key>
       <array>
         <string>/Users/your_username/.local/bin/kmonad</string>
         <string>/Users/your_username/.kmonad.kbd</string>
       </array>
       <key>RunAtLoad</key>
       <true />
       <key>StandardOutPath</key>
       <string>/tmp/kmonad.stdout</string>
       <key>StandardErrorPath</key>
       <string>/tmp/kmonad.stderr</string>
     </dict>
   </plist>
   ```
9. Restart or start/stop the daemon manually with these commands:
   ```shell
   sudo launchctl load -w /Library/LaunchDaemons/local.kmonad.plist
   sudo launchctl unload /Library/LaunchDaemons/local.kmonad.plist
   ```
10. If you get `§` rather than backtick it might help getting a custom input
    source. Use
    [this one](/assets/homerow-mods/US%20ISO%20HiTilde.keylayout.zip)
    ([source][apple-forums]) by copying it to `/Library/Keyboard Layouts` and
    selecting it in System Preferences > Keyboard > Input Sources.

## References
- [Precondition post on homerow mods](https://precondition.github.io/home-row-mods)
- [KMonad tutorial][kmonad-tutorial]
- [Example homerow mods config and tutorial](https://gist.github.com/amiorin/4c74f63fe599a1dcbd0933628df1aac9)
- [Post](https://nimishgautam.github.io/posts/kmonad-osx/) on how to run KMonad as a service

[apple-forums]: https://discussions.apple.com/thread/253602833?answerId=256749398022
[driver]: https://github.com/pqrs-org/Karabiner-DriverKit-VirtualHIDDevice/releases/download/v2.1.0/Karabiner-DriverKit-VirtualHIDDevice-2.1.0.pkg
[karabiner]: https://karabiner-elements.pqrs.org/
[kmonad-tutorial]: https://github.com/kmonad/kmonad/blob/master/keymap/tutorial.kbd
[kmonad]: https://github.com/kmonad/kmonad
[precondition_homerow_mods]: https://precondition.github.io/home-row-mods#what-are-home-row-mods
[qmk]: https://qmk.fm/
[zmk]: https://zmkfirmware.dev/
