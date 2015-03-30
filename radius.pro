TEMPLATE = subdirs

SUBDIRS += src \
           app

include (./radius.conf)

RESOURCES = ./icons/radius_icons.qrc

#transl
transl.files = ./transl/*.qm
transl.path = $$RAD_TRANSL_DIR
INSTALLS += transl
TRANSLATIONS = ./transl/radius.ts

unix{
    CONFIG_NAME_FILE = ./install_prefix.conf

    CURRENT_STRING = $$sprintf("printf %1RAD_PREFIX=%2%1 > %3", $$LITERAL_QUOTE, $$RAD_PREFIX, $$CONFIG_NAME_FILE)
    system($$CURRENT_STRING)
}
