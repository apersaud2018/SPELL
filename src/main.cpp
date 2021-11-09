#include <iostream>

//int main(int argc, char* argv[]) {
//	
//	std::cout << "Hello World!" << std::endl;
//	
// 	return 0;
// }

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <test.h>
#include <IntermediateDataStructure.h>
#include <views/SPELLMainWindow.h>


int main(int argc, char** argv)
{
	/*IntermediateDataStructure data;
    data.initialize("TestProject", "C:/test/project");
    data.addAudioFile("C:/test/songs/babyShark.wav");
    data.addAudioFile("C:/test/yolo.txt");
    data.addAudioFile("C:/test/crab.wav");
    helloSPELL();*/
    QApplication app(argc, argv);
	 QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
		qDebug().nospace() << "locale " << qPrintable(locale);
        const QString baseName = "spell_" + QLocale(locale).name();
		qDebug().nospace() << "base " << qPrintable(baseName);
        if (translator.load(":/i18n/" + baseName)) {
			qDebug().nospace() << "found " << qPrintable(baseName);
            app.installTranslator(&translator);
            break;
        }
    }
	
    SPELLMainWindow widget;
	
    widget.show();
    return app.exec();
}
