import os

au = defines.get("au")  # type: ignore
clap = defines.get("clap")  # type: ignore
lv2 = defines.get("lv2")  # type: ignore
vst3 = defines.get("vst3")  # type: ignore

files = [au, clap, lv2, vst3]

background = "#efefef"

symlinks = {
    "AU": "/Library/Audio/Plug-ins/Components",
    "CLAP": "/Library/Audio/Plug-ins/CLAP",
    "LV2": "/Library/Audio/Plug-ins/LV2",
    "VST3": "/Library/Audio/Plug-ins/VST3",
}

arrange_by = None
grid_offset = (0, 0)
grid_spacing = 100
scroll_position = (0, 0)
text_size = 12
icon_size = 70

icon_locations = {
    os.path.basename(au): (70, 50),
    os.path.basename(clap): (220, 50),
    os.path.basename(lv2): (370, 50),
    os.path.basename(vst3): (520, 50),
    "AU": (70, 170),
    "CLAP": (220, 170),
    "LV2": (370, 170),
    "VST3": (520, 170),
}
