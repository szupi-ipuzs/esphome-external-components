# esphome-external-components
External components for esphome
## [buff_log](https://github.com/szupi-ipuzs/esphome-external-bufflog)
## [mr_bridge](https://github.com/szupi-ipuzs/esphome-external-mrbridge)
## [deep_sleep_libretiny](https://github.com/szupi-ipuzs/esphome-external-lt-deepsleep)
## [uyat](https://github.com/szupi-ipuzs/esphome-external-uyat)

## Troubleshooting
If you try to import this repo as external components via the "type: git" source, and then use one of them in your yaml, you will most likely get the following error:
```
Could not find __init__.py file for component deep_sleep_libretiny. Please check the component is defined by this source (search path: /Users/user/esphome/.esphome/external_components/22631bcd/components/deep_sleep_libretiny/__init__.py).
```
This is because this repo is built using git submodules, which esphome cannot handle correctly. There's a [pending PR](https://github.com/esphome/esphome/pull/7319) that fixes this.
If you don't want to wait, you can:
* manually patch the needed files in esphome to do the same as the above PR
<br/>or
* import the components using the "type: local" source. Here are the necessary steps (git command line tools needed):

1) download [this repo](https://github.com/szupi-ipuzs/esphome-external-components) to a local directory, next to where your esphome is (use "git clone" command)
2) manually initialize the submodules ("git submodule update --init --recursive")
3) make this directory accessible to esphome. If using docker, you can mount this directory as a volume.
4) import the external components in your yaml using one of the "local" options with the above directory (see [docs](https://esphome.io/components/external_components.html))
![obraz](https://github.com/user-attachments/assets/ae54c165-1ba1-4563-b342-aa60f7562a87)

