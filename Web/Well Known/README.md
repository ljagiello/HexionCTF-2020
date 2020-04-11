# Web - Well Known

## Challenge
Well... it's known (:

https://wk.hexionteam.com

Author: Yarin

## Solution
```
➜  ~ curl https://wk.hexionteam.com
<html>
	<body>
		<h1>404 Not Found</h1>
	</body>
</html>
➜  ~ curl https://wk.hexionteam.com/robots.txt
Sitemap: sitemap.xml
Allow: *
➜  ~ curl https://wk.hexionteam.com/sitemap.xml
<?xml version="1.0" encoding="UTF-8"?>

<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">
   <url>
      <loc>https://wk.hexionteam.com/404.html</loc>
   </url>
  <url>
      <loc>https://wk.hexionteam.com/robots.txt</loc>
   </url>
  <url>
      <loc>https://wk.hexionteam.com/.well-known/security.txt</loc>
   </url>
</urlset>
➜  ~ curl https://wk.hexionteam.com/.well-known/security.txt
Flag: hexCTF{th4nk_y0u_liv3_0v3rfl0w}
```

## Flag
```
hexCTF{th4nk_y0u_liv3_0v3rfl0w}
```
