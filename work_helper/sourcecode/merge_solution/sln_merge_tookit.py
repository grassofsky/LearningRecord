import PySimpleGUI as sg 
import os
import subprocess
import pickle

def getAllSlnFiles(path, topdown=False):
    slnFiles = []
    for root, dirs, files in os.walk(path):
        for file in files:
            if file.endswith('.sln'):
                slnFiles.append(os.path.join(root, file))
    return slnFiles


def copyIncludeFiles(sourceSlnList, destSln):
    destDir = os.path.join(os.path.dirname(destSln), 'include\\')
    if not os.path.isdir(destDir):
        os.makedirs(destDir)
    
    sourceSlnDirList = [os.path.dirname(sourceSln) for sourceSln in sourceSlnList]
    for sourceSlnDir in sourceSlnDirList:
        sourceIncludeDir = os.path.join(sourceSlnDir, 'include')
        if os.path.isdir(sourceIncludeDir):
            cmdline = 'xcopy ' + sourceIncludeDir + " " + destDir + " /S /Y"
            print(cmdline)
            os.system(cmdline)

sg.theme('DarkAmber')

dumpfile = 'configdump.pickle'
sln_wait_for_merge = []
sln_for_merge = []
input_size = (100,1)
listbox_size = (140,10)
window_size = (1100,600)
layout = [
    [sg.Text('合并后的解决方案(路径+文件名)：'), sg.Input(key='-RESULT-FILE-', size=input_size)],
    [sg.Text('待合并的解决方案目录：'), sg.Input(key='-INPUT-DIR-', size=input_size),sg.Button('Update')],
    [sg.Text('待选解决方案：')],
    [sg.Listbox(values=sln_wait_for_merge, size=listbox_size, key='-WAIT-', enable_events=True)],
    [sg.Text('选中的解决方案：')],
    [sg.Listbox(values=sln_for_merge, size=listbox_size, key='-READY-', enable_events=True)],
    [sg.Button('合并',key='-BT-MERGE-'), sg.Button('更新include目录',key='-BT-UPDATE-INCLUDE-')],
    [sg.Button('导出配置文件',key='-BT-EXPORT-'), sg.Button('导入配置文件',key='-BT-IMPORT-')]
]

window = sg.Window('Sln 合并小助手', layout, size=window_size)
merge_sln_exe = 'merge-solutions.exe'
while True:
    event, values = window.read()
    if event in (None,):
        break
    if event in ('Update',):
        if values['-INPUT-DIR-']:
            path = values['-INPUT-DIR-']
            if not os.path.isdir(path):
                sg.popup("请输入正确的目录！")
                continue
            sln_wait_for_merge = getAllSlnFiles(path)
            window['-WAIT-'].update(sln_wait_for_merge)
    elif event in ('-WAIT-',):
        sln_for_merge = sln_for_merge + values['-WAIT-']
        window['-READY-'].update(sln_for_merge)
        sln_wait_for_merge.remove(values['-WAIT-'][0])
        window['-WAIT-'].update(sln_wait_for_merge)
    elif event in ('-READY-',):
        sln_wait_for_merge= sln_wait_for_merge + values['-READY-']
        window['-WAIT-'].update(sln_wait_for_merge)
        sln_for_merge.remove(values['-READY-'][0])
        window['-READY-'].update(sln_for_merge)
    elif event in ('-BT-MERGE-',):
        if not os.path.isfile(merge_sln_exe):
            sg.popup("可执行文件" + merge_sln_exe + "不在PATH中")
            continue
        if not values['-RESULT-FILE-']:
            sg.popup("请输入merge后的输出sln文件名，带路径")
            continue
        p = subprocess.Popen(args=[merge_sln_exe, '/out', values['-RESULT-FILE-']] + sln_for_merge, stdout=subprocess.PIPE)
        copyIncludeFiles(sln_for_merge, values['-RESULT-FILE-'])
        sg.popup("已完成合并！")
    elif event in ('-BT-UPDATE-INCLUDE-',):
        if not values['-RESULT-FILE-']:
            sg.popup("请输入merge后的输出sln文件名，可带路径")
            continue
        copyIncludeFiles(sln_for_merge, values['-RESULT-FILE-'])
        sg.popup("已完成拷贝！")
    elif event in ('-BT-EXPORT-',):
        input_dir = values['-INPUT-DIR-']
        result_file = values['-RESULT-FILE-']
        params = [result_file, input_dir, sln_wait_for_merge, sln_for_merge]
        with open(dumpfile, 'wb') as fid:
            pickle.dump(params, fid, -1)
    elif event in ('-BT-IMPORT-',):
        with open(dumpfile, 'rb') as fid:
            params = pickle.load(fid)
            result_file = params[0]
            input_dir = params[1]
            sln_wait_for_merge = params[2]
            sln_for_merge = params[3]
            window['-READY-'].update(sln_for_merge)
            window['-WAIT-'].update(sln_wait_for_merge)
            window['-INPUT-DIR-'].update(input_dir)
            window['-RESULT-FILE-'].update(result_file)


window.close()