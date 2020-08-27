#! /bin/bash
pushd . >/dev/null
cd ~/workspace/trunk/LogServer
svn -m "" ci
svn up
if [ -e Makefile ]
then
    make distclean -s && \
    rm -fR GeneratedFiles release debug moc_* qrc_* ui_* *.o 2> /dev/null
fi
QT_OTIMIZA="linux-g++"
MAQUINA=`uname -m`
if [ "$MAQUINA" = "x86_64" ]
then
        QT_OTIMIZA="linux-g++-64"
fi
echo -e - - -Compilando o programa '\033[1;33m'LogServer'\033[0m' de envio de registros de depuração, para a plataforma '\033[1;33m'$MAQUINA'\033[0m' ...
if [ "$MAQUINA" = "x86_64" ]
then
        /usr/lib64/qt5/bin/qmake -r -spec $QT_OTIMIZA "CONFIG-=debug_and_release release debug" "CONFIG+=release" 2> ~/lixo.txt && /usr/bin/make qmake_all
else
        /usr/lib/qt5/bin/qmake -r -spec $QT_OTIMIZA "CONFIG-=debug_and_release release debug" "CONFIG+=release" 2> ~/lixo.txt && /usr/bin/make qmake_all
fi
# lrelease TesteSerial.pro
make $MAKEOPTS -s && \
/usr/bin/strip -s LogServer

# if [ "$MAQUINA" = "x86_64" ]
# then
#	/usr/bin/upx --best --ultra-brute --all-methods --all-filters --preserve-build-id LogServer
# fi
mv LogServer ~/bin/ && \
make $MAKEOPTS distclean -s && \
rm -fR GeneratedFiles debug release moc_* qrc_* ui_* *.o 2> /dev/null
if [ -e /usr/bin/cloc ]
then
    /usr/bin/cloc *.cpp *.h
fi
popd >/dev/null
echo -e - - - Finalizando a compilação do programa '\033[1;33'mLogServer'\033[0m' de envio de registros de depuração, para a plataforma '\033[1;33m'$MAQUINA'\033[0m' .
