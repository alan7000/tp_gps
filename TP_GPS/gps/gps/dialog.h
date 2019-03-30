#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QNetworkProxy>
#include <QWebView>
#include <QWebFrame>
#include <QTimer>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QStringList>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include "math.h"
#include "stdio.h"

#define MAP_HTML "<html><head><script type=\"text/javascript\" " \
    "src=\"http://maps.google.com/maps/api/js?sensor=false\"></script>" \
    "<script  type=\"text/javascript\">" \
    "   var map;" \
    "   var latlng;" \
    "   var currentPosition = null;"\
    "   var previousPosition = null;"\
    "   var pos = null;"\
    "   var myOptions = {zoom: 13, center: latlng, mapTypeId: google.maps.MapTypeId.ROADMAP};"\
    "   function traceParcours(){"\
    "       var newLineCoordinates = [ currentPosition , previousPosition ];" \
    "       var newLine = new google.maps.Polyline({" \
    "       path: newLineCoordinates," \
    "       strokeColor: \"red\"," \
    "       strokeOpacity: 1.0," \
    "       strokeWeight: 2," \
    "       fillOpacity: 0" \
    "       });" \
    "       newLine.setMap(map);}" \
    "    function placeMarker(){"\
    "       var location = new google.maps.LatLng(pos);"\
    "       var marker = new google.maps.Marker({position: pos, map: map});}"\
    "</script>" \
    "</head><body style=\"margin:0px; padding:0px;\">" \
    "<div id=\"map_canvas\" style=\"width:100%; height:100%\"></div>" \
    "</body></html>"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    QNetworkProxy proxy;

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    void initProxy();
    void initMap(void);
    bool chargementDonnee(QString map);
    void placeMarker(qreal lat, qreal lon);
    double distanceParcours();
    double degToRad(double db);
    qreal lat;
    qreal lon;
    QList<qreal> listLat;
    QList<qreal> listLon;
    QList<qreal> listAlt;
    QList<QString> listHor;

public slots:
    void loading(void);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // DIALOG_H
