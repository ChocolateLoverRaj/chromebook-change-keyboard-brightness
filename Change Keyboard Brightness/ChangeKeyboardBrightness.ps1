# This program works! I actually wrote this program!
param (
    [string]$changePercent
)

$ectoolPath = "C:\Program Files\crosec\ectool"

# Get current keyboard brightness
# Set up the Start-Process parameters
$processStartInfo = New-Object System.Diagnostics.ProcessStartInfo
$processStartInfo.FileName = $ectoolPath
$processStartInfo.RedirectStandardOutput = $true
$processStartInfo.UseShellExecute = $false
$processStartInfo.Arguments = "pwmgetkblight"

# Start the process
$process = New-Object System.Diagnostics.Process
$process.StartInfo = $processStartInfo
$process.Start() | Out-Null

# Get the captured output
$output = $process.StandardOutput.ReadToEnd()

# Wait for the process to complete
$process.WaitForExit()

# Display the output
Write-Output $output

# Get the number from the string output
$regex = [regex]::Match($output, '\d+')
if ($regex.Success) {
    $currentBacklightPercent = [int]$regex.Value
} else {
    Write-Output "Failed to extract the backlight percent from the output."
    exit
}

# Add the value of the first argument to the extracted number
$newBacklightPercent = $currentBacklightPercent + [int]$changePercent
if ($newBacklightPercent -ge 100) {
    $newBacklightPercent = 100
} 
if ($newBacklightPercent -le 0) {
    $newBacklightPercent = 0
}

Write-Output "Current keyboard backlight percent: $currentBacklightPercent"
Write-Output "New keyboard backlight percent: $newBacklightPercent"

# Set the new keyboard brightness
# Set up the Start-Process parameters
$processStartInfo = New-Object System.Diagnostics.ProcessStartInfo
$processStartInfo.FileName = $ectoolPath
$processStartInfo.RedirectStandardOutput = $true
$processStartInfo.UseShellExecute = $false
$processStartInfo.Arguments = "pwmsetkblight $newBacklightPercent"

# Start the process
$process = New-Object System.Diagnostics.Process
$process.StartInfo = $processStartInfo
$process.Start() | Out-Null

$output = $process.StandardOutput.ReadToEnd()

Write-Output $output
