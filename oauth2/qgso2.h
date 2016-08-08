/***************************************************************************
    begin                : August 1, 2016
    copyright            : (C) 2016 by Boundless Spatial, Inc. USA
    author               : Larry Shaffer
    email                : lshaffer at boundlessgeo dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSO2_H
#define QGSO2_H

#include "o2.h"

class QgsAuthOAuth2Config;

/// QGIS-specific subclass of O2 lib's base OAuth 2.0 authenticator
class QgsO2: public O2
{

    Q_OBJECT

  public:
    explicit QgsO2( const QString &authcfg, QgsAuthOAuth2Config *oauth2config = 0,
                    QObject *parent = 0, QNetworkAccessManager *manager = 0 );

    ~QgsO2();

    QString authcfg() const { return mAuthcfg; }
    QgsAuthOAuth2Config *oauth2config() { return mOAuth2Config; }

  public slots:
    void clearProperties();

  private:
    void initOAuthConfig();

    QString mAuthcfg;
    QgsAuthOAuth2Config *mOAuth2Config;

};

#endif // QGSO2_H
