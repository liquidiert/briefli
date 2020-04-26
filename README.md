# briefli
Linux text expander. Heavily inspired by breevy and built from [keylog](https://github.com/SCOTPAUL/keylog) source.

## Installation
In order to work briefli needs the installation of redis first:

```bash
sudo apt update
sudo apt install redis
```

After installation set redis up (for configuration options see [redis conf](https://redis.io/topics/config)): 

```bash
sudo nano /etc/redis/redis.conf
redis-server /etc/redis/redis.conf
```

Now, if not done already, clone the repo and execute:
```bash
make
sudo make install
```
If you ever want to uninstall briefli just exec:
```bash
sudo make uninstall
```
## Usage
As mentioned earlier briefli works with redis. That is so one can dynamically add / change / delete shortcuts. For general use of redis see [redis commands](https://redis.io/commands).  

**Currently shortcuts can be 5 chars long max.**

### Adding a shortcut
```bash
redis-cli set <shortcut> <shortcut_value>
```

### Changing a shortcut
```bash
redis-cli set <shortcut> <new_shortcut_value>
```

### Removing a shortcut
```bash
redis-cli del <shortcut>
```

To start briefli just type `briefly` into your console and see your shortcuts come to life :D  
brieflis' default behaviour is to trigger text replace **after** you pressed CTRL key.

## Options
There a several options that allow you to tweak the behaviour of briefli.  

For example you could add a systemd service and let it execute `briefli -d` and briefli will run for ever as a background service :D  
For all ooptions see `briefly -h`