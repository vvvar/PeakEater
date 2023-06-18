files = [
    defines.get("au"),  # type: ignore
    defines.get("clap"),  # type: ignore
    defines.get("lv2"),  # type: ignore
    defines.get("vst3"),  # type: ignore
]

background = "#3344ff"

symlinks = {
    "AU": "/Library/Audio/Plug-ins/Components",
    "CLAP": "/Library/Audio/Plug-ins/CLAP",
    "LV2": "/Library/Audio/Plug-ins/LV2",
    "VST3": "/Library/Audio/Plug-ins/VST3",
}
