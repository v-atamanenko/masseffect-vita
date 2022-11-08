import glob
import subprocess

sce_vp_psp2 = []
sce_fp_psp2 = []

def gettargetname(file):
    res = file.replace("cg/", "gxp/")
    res = res.replace("cg\\", "gxp\\")
    res = res.replace(".cg", ".gxp")
    return res

for file in glob.glob("cg/*.cg"):
    with open(file) as f:
        first_line = f.readline()
        if "sce_fp_psp2" in first_line:
            sce_fp_psp2.append(file)
        elif "sce_vp_psp2" in first_line:
            sce_vp_psp2.append(file)
        else:
            print("Malformed cg shader: no profile header found\n")

for shader in sce_vp_psp2:
    subprocess.run(["psp2cgc.exe", "-profile", "sce_vp_psp2", shader, "-W4", "-Wperf", "-fastprecision", "-O4", "-o", gettargetname(shader)])

for shader in sce_fp_psp2:
    subprocess.run(["psp2cgc.exe", "-profile", "sce_fp_psp2", shader, "-W4", "-Wperf", "-fastprecision", "-O4", "-o", gettargetname(shader)])
