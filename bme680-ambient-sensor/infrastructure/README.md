# InfluxDB + Grafana Setup

This Docker Compose setup provides a complete time series database solution with InfluxDB and Grafana.

## Services

- **InfluxDB 2.7**: Time series database running on port 8086
- **Grafana 10.2.0**: Visualization platform running on port 3000

## Quick Start

1. Start the services:
   ```bash
   docker-compose up -d
   ```

2. Access the services:
   - **Grafana**: http://localhost:3000
     - Username: `admin`
     - Password: `admin`
   - **InfluxDB**: http://localhost:8086
     - Username: `admin`
     - Password: `admin`
     - Organization: `myorg`
     - Bucket: `mybucket`
     - Token: `my-super-secret-auth-token`

## Configuration

### InfluxDB
- Organization: `myorg`
- Default bucket: `mybucket`
- Admin token: `my-super-secret-auth-token`

### Grafana
- The InfluxDB data source is automatically configured
- Uses Flux query language (InfluxDB 2.x)
- Connected to the default bucket `mybucket`

## Data Persistence

All data is persisted using Docker volumes:
- `influxdb_data`: InfluxDB data storage
- `influxdb_config`: InfluxDB configuration
- `grafana_data`: Grafana dashboards and settings

## Stopping the Services

```bash
docker-compose down
```

To remove all data volumes:
```bash
docker-compose down -v
```

## Writing Data to InfluxDB

You can write data to InfluxDB using the HTTP API:

```bash
curl -X POST "http://localhost:8086/api/v2/write?org=myorg&bucket=mybucket" \
  -H "Authorization: Token my-super-secret-auth-token" \
  -H "Content-Type: text/plain; charset=utf-8" \
  -d "temperature,location=room1 value=23.5"
```

## Creating Dashboards

1. Log into Grafana at http://localhost:3000
2. Go to Dashboards → New Dashboard
3. Add a new panel
4. Select "InfluxDB" as the data source
5. Use Flux queries to visualize your time series data 