@echo off 
setlocal enabledelayedexpansion
for /l %%x in (44, -1, 35) do (
	set /a var= "B=%%x"
	set /a var*=!var!
	START /wait /b raytracer.exe example.json output-%%x.ppm !var!
	
	)