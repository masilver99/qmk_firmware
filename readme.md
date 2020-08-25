# Fork of the Quantum Mechanical Keyboard Firmware

NOTE:  This is a fork of the qmk repo.  The purpose of the fork is to add some custom converters as well as explore adding a 16x2 LCD screen.  Since my C skills are relatively weak, forking was the best option.

I added, rudimentarily, code to count keystrokes, store and load the count from the EEPROM as well as display the num lock, caps lock and click count on a small 16x LCD on a 122-key IBM terminal keyboard.

C isn't my primary language, so all constructive criticism is welcome.

Qmk's original readme:
This is a keyboard firmware based on the [tmk\_keyboard firmware](http://github.com/tmk/tmk_keyboard) with some useful features for Atmel AVR and ARM controllers, and more specifically, the [OLKB product line](https://olkb.com), the [ErgoDox EZ](http://www.ergodox-ez.com) keyboard, and the [Clueboard product line](http://clueboard.co/).

## Documentation

* [See the official documentation on docs.qmk.fm](https://docs.qmk.fm)

The docs are hosted on [Gitbook](https://www.gitbook.com/book/qmk/firmware/details) and [GitHub](/docs/) (they are synced). You can request changes by making a fork and [pull request](https://github.com/qmk/qmk_firmware/pulls), or by clicking the "suggest an edit" link on any page of the docs.

## Supported Keyboards

* [Planck](/keyboards/planck/)
* [Preonic](/keyboards/preonic/)
* [ErgoDox EZ](/keyboards/ergodox_ez/)
* [Clueboard](/keyboards/clueboard/)
* [Cluepad](/keyboards/clueboard/17/)

The project also includes community support for [lots of other keyboards](/keyboards/).

## Maintainers

QMK is developed and maintained by Jack Humbert of OLKB with contributions from the community, and of course, [Hasu](https://github.com/tmk). The OLKB product firmwares are maintained by [Jack Humbert](https://github.com/jackhumbert), the Ergodox EZ by [Erez Zukerman](https://github.com/ezuk), and the Clueboard by [Zach White](https://github.com/skullydazed).

## Official website

[http://qmk.fm](http://qmk.fm) is the official website of QMK, where you can find links to this page, the documentation, and the keyboards supported by QMK.
