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

#include "qgso2.h"

#include "o0globals.h"
#include "o0settingsstore.h"
#include "qgsapplication.h"
#include "qgsauthoauth2config.h"
#include "qgslogger.h"

#include <QSettings>
#include <QUrl>


QgsO2::QgsO2( const QString &authcfg, QgsAuthOAuth2Config *oauth2config,
              QObject *parent , QNetworkAccessManager *manager )
    : O2( parent, manager )
    , mAuthcfg( authcfg )
    , mOAuth2Config( oauth2config )
{
  initOAuthConfig();
}

QgsO2::~QgsO2()
{
  // FIXME: This crashes app on QgsApplication destruction
  //        Verify that objects are acutally being deleted via QgsAuthManager's destruction
//  mOAuth2Config->deleteLater();
}

void QgsO2::initOAuthConfig()
{
  if ( !mOAuth2Config )
  {
    return;
  }

  // common properties to all grant flows
  QString localpolicy = QString( "http://127.0.0.1:% 1/%1" ).arg( mOAuth2Config->redirectUrl() ).replace( "% 1", "%1" );
  QgsDebugMsg( QString( "localpolicy(w/port): %1" ).arg( QString( localpolicy ).arg( mOAuth2Config->redirectPort() ) ) );
  setLocalhostPolicy( localpolicy );
  setLocalPort( mOAuth2Config->redirectPort() );

  setTokenUrl( mOAuth2Config->tokenUrl() );
  setRefreshTokenUrl( mOAuth2Config->refreshTokenUrl() );

  setScope( mOAuth2Config->scope() );
  // TODO: add support to O2 (or this class?) for state query param

  // common optional properties
  setApiKey( mOAuth2Config->apiKey() );
  setExtraRequestParams( mOAuth2Config->queryPairs() );

  O2::GrantFlow o2flow;
  switch ( mOAuth2Config->grantFlow() )
  {
    case QgsAuthOAuth2Config::AuthCode:
      o2flow = O2::GrantFlowAuthorizationCode;
      setRequestUrl( mOAuth2Config->requestUrl() );
      setClientId( mOAuth2Config->clientId() );
      setClientSecret( mOAuth2Config->clientSecret() );

      break;
    case QgsAuthOAuth2Config::Implicit:
      o2flow = O2::GrantFlowImplicit;
      setRequestUrl( mOAuth2Config->requestUrl() );
      setClientId( mOAuth2Config->clientId() );

      break;
    case QgsAuthOAuth2Config::ResourceOwner:
      o2flow = O2::GrantFlowResourceOwnerPasswordCredentials;
      setClientId( mOAuth2Config->clientId() );
      setClientSecret( mOAuth2Config->clientSecret() );
      setUsername( mOAuth2Config->username() );
      setPassword( mOAuth2Config->password() );

      break;
  }
  setGrantFlow( o2flow );

  if ( mOAuth2Config->persistToken() )
  {
    QSettings* settings = new QSettings( QgsApplication::qgisSettingsDirPath() + "/oauth2-cache.ini",
                                         QSettings::IniFormat );
    O0SettingsStore *store = new O0SettingsStore( settings, O2_ENCRYPTION_KEY );
    store->setGroupKey( QString( "authcfg_%1" ).arg( mAuthcfg ) );
    setStore( store );
  }
}

// slot
void QgsO2::clearProperties()
{
  // TODO: clear object properties
}
