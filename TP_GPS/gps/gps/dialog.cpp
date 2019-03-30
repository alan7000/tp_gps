#include "dialog.h"
#include "ui_dialog.h"
#include "error.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

void Dialog::initProxy(){ // nécessaire si accès internet via proxy
    proxy.setType(QNetworkProxy::Socks5Proxy);
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("172.16.254.254");
    proxy.setPort(3128);
    // proxy.setUser("username");
    // proxy.setPassword("password");
    QNetworkProxy::setApplicationProxy(proxy);
}

void Dialog::initMap(void){ // initialise le QWebView avec du code HTML
    QString contenu;
    // Autoriser l'exécution de code javascript
    ui->webView->settings()->setAttribute(QWebSettings::JavascriptEnabled, true );
    contenu = MAP_HTML; // code javascript prédéfini dans MAP_HTML
    ui->webView->page()->mainFrame()->setHtml(contenu); // chargement de la page
    QTimer::singleShot(1000, this, SLOT(loading()));
}



void Dialog::loading(void){ // charge la carte dont le centre est lat,lon
    QString code = QString("latlng=new google.maps.LatLng(%1, %2);").arg(lat).arg(lon);
    code += "map = new google.maps.Map(document.getElementById(\"map_canvas\"),myOptions);";
    code += "map.setZoom(15);map.setCenter(latlng);";
    ui->webView->page()->mainFrame()->evaluateJavaScript(code);
}

bool Dialog::chargementDonnee(QString map)
{
    QFile file;
    QStringList liste;
    QString lattemp, lontemp, alttemp, hortemp;
    file.setFileName(map); //selectionne le fichier
    file.open(QIODevice::ReadOnly);
    listAlt.clear();
    listHor.clear();
    listLat.clear();
    listLon.clear();
    while (file.atEnd() != true) {
        QByteArray line = file.readLine();
        QString ligne = line;
        liste = ligne.split(",",QString::SkipEmptyParts);
        lattemp = liste.at(2);
        lontemp = liste.at(3);
        hortemp = liste.at(5);
        alttemp = liste.at(6);
        listLat += lattemp.toDouble();
        listLon += lontemp.toDouble();
        listHor += hortemp;
        listAlt += alttemp.toDouble();
//        qDebug() << ligne;
//        qWarning("Lat = %04f", lattemp.toDouble());
//        qWarning("Lon = %04f", lontemp.toDouble());
//        qWarning("Hor = %s", qUtf8Printable(hortemp));
//        qWarning("Alt = %04f", alttemp.toDouble());
//        qWarning("%s", qUtf8Printable(ligne));
    }
    lat = listLat[0];
    lon = listLon[0];
    file.close();
    qDebug() << lat;
    qDebug() << lon;
    //placeMarker(lat, lon);
    return 1;
}

void Dialog::placeMarker(qreal lat, qreal lon)
{
    QString code = QString("pos = new google.maps.LatLng(%1, %2);").arg(lat).arg(lon);
    code += "placeMarker();";
    ui->webView->page()->mainFrame()->evaluateJavaScript(code);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr("Ouvrir fichier texte (*.txt)"), "C:/Users/Jupiter/Documents/Alan_Flambard/TP_GPS/gps/build-gps-Desktop_Qt_5_5_1_MinGW_32bit-Debug/maps", tr("Ouvrir fichier texte (*.txt)"));
    if (filename == 0) {
        QMessageBox msgBox;
        msgBox.setText("Veuillez selectionner un fichier Map !!!");
        msgBox.exec();
    }
    else {
        chargementDonnee(filename);
        //    QFileDialog *fildia;
        //    QString stri;
        //    fildia = new QFileDialog();
        //    fildia->show();
        //    stri = fildia->getOpenFileName();
        //    chargementDonnee(stri);
        lat = listLat.at(0);
        lon = listLon.at(0);
        initMap();
    }


}

void Dialog::on_pushButton_2_clicked()
{
    int longeur = listLat.length();
    double latpre, lonpre, latcur, loncur;
    placeMarker(lat, lon);
    for (int i = 1; i < longeur; i++) {
        latpre = listLat.at(i-1);
        lonpre = listLon.at(i-1);
        latcur = listLat.at(i);
        loncur = listLon.at(i);
        QString code = QString("previousPosition = new google.maps.LatLng(%1, %2);").arg(latpre).arg(lonpre);
        code += QString("currentPosition = new google.maps.LatLng(%1, %2);").arg(latcur).arg(loncur);
        code += "traceParcours();";

        ui->webView->page()->mainFrame()->evaluateJavaScript(code);

    }
    QString distance = "distance parcourue = " + QString::number(distanceParcours()).left(4) + " km";
    ui->lineEdit->setText(distance);
}

double Dialog::distanceParcours(){
    int i, taille = listAlt.length()-1, r = 6366;
    qreal metre = 0, d = 0, distance;
    double lat1, lon1, lat2, lon2, alt1, alt2;
    for(i=1; i<taille; i++){
        lat1 = listLat.at(i-1);
        lat2 = listLat.at(i);
        lon1 = listLon.at(i-1);
        lon2 = listLon.at(i);
        alt1 = listAlt.at(i-1);
        alt2 = listAlt.at(i);

        lat1 = degToRad(lat1);
        lat2 = degToRad(lat2);
        lon1 = degToRad(lon1);
        lon2 = degToRad(lon2);

        metre = metre + (2*asin( sqrt((sin((lat1 - lat2)/2))*(sin((lat1 - lat2)/2))) + cos(lat1)*cos(lat2)* ((sin((lon1-lon2)/2))*(sin((lon1-lon2)/2)))));

        d = metre * r;

        distance = sqrt(d*d) + ((alt2 - alt1)*(alt2 - alt1));

    }
    return distance;
}

double Dialog::degToRad(double db){
    db = (db*M_PI)/180;
    return db;
}

//QString Dialog::dureeParcours(void){
//    /*A FAIRE*/
//    hordb = listHor.at(y);
//    horfn = listHor.at(x);

//    valdb = hordb.toInt();
//    valfn = horfn.toInt();
//   // qWarning("Valdb = %d",valdb);
//    //qWarning("Valfn = %d",valfn);

//    valc = valfn-valdb;
//    /*Fini*/

//}
