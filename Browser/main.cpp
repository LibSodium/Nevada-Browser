#include <QFile>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QWebEngineView>
#include <QWebEngineProfile>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWebEngineView *web_view = new QWebEngineView;
    QPushButton *button = new QPushButton("Get HTML");
    button->setEnabled(false);
    QObject::connect(button, &QPushButton::clicked, [web_view]()
                     {
                         web_view->page()->runJavaScript("document.documentElement.outerHTML", [](const QVariant &data)
                                                         {
                                                             QFile file("page_html_code.html");
                                                             if(!file.open(QFile::WriteOnly | QFile::Text)){
                                                                 qDebug() << "Cannot create a file";
                                                                 return;
                                                             }
                                                             QTextStream stream(&file);
                                                             stream << data.toString();
                                                             file.close();
                                                         });
                     });
    QObject::connect(web_view, &QWebEngineView::loadFinished, [button](){button->setEnabled(true);});
    web_view->load(QUrl("https://google.com"));
    QWidget main_window;
    QVBoxLayout *lay = new QVBoxLayout(&main_window);
    lay->addWidget(button);
    lay->addWidget(web_view);
    main_window.resize(640, 480);
    main_window.show();
    return app.exec();
}
