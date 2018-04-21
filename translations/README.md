# /translations/ #

How to add or update a translation (Windows):

1) Run `update.cmd` to call <QT_DIR>/bin/lupdate.exe to extract translatable texts to .ts (from source files).

2) Run `linguist.cmd` to bring up <QT_DIR>/bin/linguist.exe to edit .ts file.

3) Run `release.cmd` to call <QT_DIR>/bin/lrelease.exe to compile .ts to .qm.

4) Run `../deploy.cmd` to have all .qm files copied to /deploy/translations/ directory.
