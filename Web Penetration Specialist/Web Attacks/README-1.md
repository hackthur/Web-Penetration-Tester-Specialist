


1.- Identify the web application
2.- Try to access and use this command 
```bash

curl -s -X OPTIONS http://<server ip>:<port>
```
3.- in the authentication panel, try to use burpsuite to see what happened behind the web applcation
4.- If you can see if the `admin/reset.php` request for credentials (basic authentication) we need to change the request method
from GET to POST and see what is the behaviour from the web application
5.- And after checking the web application `admin/reset.php` show different behaviour changing from the POST to HEAD
    so, return to the `index.php` web  portal and you will be able to see the flag
```bash
HTB{4lw4y5_c0v3r_4ll_v3rb5}
```

Attack method
Verb tampering Attack
