# autogeoreference

Autogeoreference is web service that performs automatic georeferencing between two similar maps. The whole process
consists from two steps. At first an affine transformation matrix is computed between maps and then the matrix is applied
to georeferenced points from georeferenced map. The second process causes that all points from the first map are
reused in the second map. You can reuse manually entered points in other maps without any effort.

## Requirements

* [Docker](https://www.docker.com/)

## Installation

On debian based systems you can create init script. Create file /etc/init.d/autogeoreference with following content

```bash
#!/bin/bash

### BEGIN INIT INFO
# Provides:          autogeoreference
# Required-Start:    docker
# Required-Stop:     docker
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Autogeoreference application
# Description:       Runs docker image.
### END INIT INFO

LOCK=/var/lock/autogeoreference
CONTAINER_NAME=autogeoreference
IMAGE_NAME=moravianlibrary/autogeoreference

case "$1" in
  start)
    if [ -f $LOCK ]; then
      echo "Autogeoreference is running yet."
    else
      touch $LOCK
      echo "Starting autogeoreference.."
      docker run --name $CONTAINER_NAME -p 127.0.0.1:9100:80 $IMAGE_NAME &
      echo "[OK] Autogeoreference is running."
    fi
    ;;
  stop)
    if [ -f $LOCK ]; then
      echo "Stopping autogeoreference.."
      rm $LOCK \
        && docker kill $CONTAINER_NAME \
        && docker rm $CONTAINER_NAME \
        && echo "[OK] Autogeoreference is stopped."
    else
      echo "Autogeoreference is not running."
    fi
    ;;
  restart)
    $0 stop
    $0 start
  ;;
  status)
    if [ -f $LOCK ]; then
      echo "Autogeoreference is running."
    else
      echo "Autogeoreference is not running."
    fi
  ;;
  update)
    docker pull $IMAGE_NAME
    $0 restart
  ;;
  *)
    echo "Usage: /etc/init.d/autogeoreference {start|stop|restart|status|update}"
    exit 1
    ;;
esac

exit 0
```

After it run these commands

```
# chmod 755 /etc/init.d/autogeoreference
# update-rc.d autogeoreference defaults
```

Now the autogeoreference service will be automatically start at server startup. You can start the service by

```
# /etc/init.d/autogeoreference start
```

The service listen on port 9100. You should install and setup apache http server, which will forward requests to the docker container.

```
# apt-get update
# apt-get install apache2
```

In directory /etc/apache2/sites-available create file with name autogeoreference.mzk.cz and with content:

```
<VirtualHost *:80>
        ServerName autogeoreference.mzk.cz

        <IfModule mod_rewrite.c>
              RewriteEngine on
              Options +FollowSymlinks

              ProxyPassMatch ^/(.*)$  http://localhost:9100/$1
              ProxyPass / http://localhost:9100
        </IfModule>
        
        ErrorLog ${APACHE_LOG_DIR}/autogeoreference-error.log

        # Possible values include: debug, info, notice, warn, error, crit,
        # alert, emerg.
        LogLevel warn

        CustomLog ${APACHE_LOG_DIR}/autogeoreference-access.log combined
</VirtualHost>
```

Enable new settings and reload apache server.

```
# a2ensite imagesearch.mzk.cz
# /etc/init.d/apache2 reload
```

## API

```
/v1/autogeoreference?georeferenced=3cK6aDVjhxWPYh1KmmPWor/201105160722-GpK3267&similar=uXy8mURggpom0KkqHY6p9/201105161057-JAcX599&draw=true&debug=true
```

* Service communicates over REST API, see example above
* Parameters:
  - georeferenced - Georeferencer ID of georeferenced map. Example: For map http://staremapy.georeferencer.cz/map/3cK6aDVjhxWPYh1KmmPWor/201105160722-GpK3267/ the ID is 3cK6aDVjhxWPYh1KmmPWor/201105160722-GpK3267
  - similar - Georeferencer ID of similar map. This is the map to which the points from georeferenced map are applied.
  - draw - If this optional parameter is set on true, the response will contain images, on which the points will be drawn. The images are encoded in Base64 encoding.
  - debug - If this optional parameter is set on true, the response will contain image, which shows how the transformation matrix was computed. The image is encoded in Base64 encoding.

## Build and run

If you want to build and run the application locally, you just use provided Makefile.

```
$ make
```
