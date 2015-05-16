This module is a small helper when using deploy modules. It solves some tiny but annoying problems:

- when enabling a module using drush the dependency will be written automatically to the deploy_module.info
- when disabling a module first the modules dependency will be removed from the deploy module so that drush doesn't disable the deploy module.
