@echo off
set maxbytesize=2097152
del /q "%~dp0\stage\*.*"

setlocal
set file="%~dp0input\Patient_01.obj"
FOR /F "usebackq" %%A IN ('%file%') DO set size=%%~zA

set /p "answer=Click Y to decimate Patient_01 (if not too small already). Click N otherwise. (Y/N)"
if /I "%answer%" EQU "N" goto :no1
echo "Yes"
"%~dp0bin\MeshLab\meshlabserver" -i "%~dp0input\Patient_01.obj" -o "%~dp0stage\Patient_01.obj" -s "%~dp0bin\MeshLab\scripts\QuadricEdgeDecimation.mlx"
goto :continue1

:no1
echo "No"
copy "%~dp0input\Patient_01.obj" "%~dp0stage\Patient_01.obj"

:continue1
echo "continue1"
endlocal

setlocal
set file="%~dp0input\Patient_02.obj"
FOR /F "usebackq" %%A IN ('%file%') DO set size=%%~zA

set /p "answer2=Click Y to decimate Patient_02 (if not too small already). Click N otherwise. (Y/N)"
if /I "%answer2%" EQU "N" goto :no2
echo "Yes"
"%~dp0bin\MeshLab\meshlabserver" -i "%~dp0input\Patient_02.obj" -o "%~dp0stage\Patient_02.obj" -s "%~dp0bin\MeshLab\scripts\QuadricEdgeDecimation.mlx"
goto :continue2

:no2
echo "No"
copy "%~dp0input\Patient_02.obj" "%~dp0stage\Patient_02.obj"

:continue2
echo "continue2"
endlocal

"%~dp0bin\CPDPreProcessing\PreprocessingData" "%~dp0stage\Patient_01.obj" "%~dp0stage\Patient_01.txt"
"%~dp0bin\NormativePreProcessing\NormativePreProcessing" "%~dp0stage\Patient_01.obj" "%~dp0stage\01_connect.txt"
"%~dp0bin\NormativePreProcessing\NormativePreProcessing" "%~dp0stage\Patient_02.obj" "%~dp0stage\02_connect.txt"

"%~dp0bin\CPDPreProcessing\PreprocessingData" "%~dp0stage\Patient_02.obj" "%~dp0stage\Patient_02.txt"
"%~dp0bin\CPDRegistration\patient_Registration" "%~dp0stage\Patient_01.txt" "%~dp0stage\Patient_02.txt" "%~dp0stage\02_to_01.txt" "%~dp0stage\Update_02.txt"
"%~dp0bin\CPDPostProcessing\CPDPostProcessing" "%~dp0stage\Update_02.txt" "%~dp0stage\02_connect.txt" "%~dp0stage\Update_02.obj"

"%~dp0bin\ComputeCurvatures\PCL_Project" "%~dp0stage\Update_02" "%~dp0stage\Patient_01" "%~dp0stage\02_to_01"
"%~dp0bin\Change3Dfile\Change3Dfile" "%~dp0stage\Patient_01" "%~dp0stage\Update_02"
"%~dp0bin\ComparativePreProcessing\CPDPostProcessing" "%~dp0files\00Header.txt" "%~dp0stage\Update_02.txt" "%~dp0stage\Patient_01.txt" "%~dp0stage\02_to_01.txt" "%~dp0files\1.txt"
copy "%~dp0stage\Patient_01_mean_curvature.txt" "%~dp0files\1_target_mean.txt"
copy "%~dp0stage\Patient_01_Gaussian_curvature.txt" "%~dp0files\1_target_Gaussian.txt"
copy "%~dp0stage\Patient_01_min_curvature.txt" "%~dp0files\1_target_min.txt"
copy "%~dp0stage\Patient_01_max_curvature.txt" "%~dp0files\1_target_max.txt"
copy "%~dp0stage\Update_02_mean_curvature.txt" "%~dp0files\1_source_mean.txt"
copy "%~dp0stage\Update_02_Gaussian_curvature.txt" "%~dp0files\1_source_Gaussian.txt"
copy "%~dp0stage\Update_02_min_curvature.txt" "%~dp0files\1_source_min.txt"
copy "%~dp0stage\Update_02_max_curvature.txt" "%~dp0files\1_source_max.txt"

"%~dp0bin\ComparativeAnalysis\Lab" "1"
REM copy "%~dp0stage\SurfaceAreaAnalysis.txt" "%~dp0output\SurfaceAreaAnalysis.txt"
REM copy "SymmetryAnalysis.txt" "%~dp0output\SymmetryAnalysis.txt"


REM del current.obj

echo "COMPARATIVE FRAMEWORK COMPLETE. Retreive Relative_Changed_Surface_Area and SymmetryAnalysis text files from Output folder."