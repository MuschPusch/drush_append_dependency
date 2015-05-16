<?php
 /**
  * @file
  * Append dependencies to deploy modules info file.
  */

/**
 * Implements drush_hook_post_COMMAND().
 *
 * Add module name to info file on module enable.
 */
function drush_drush_append_dependency_post_pm_enable($extensions) {

  if (!variable_get('deploy_module', FALSE)) {
    $module = drush_prompt(dt('No deploy module set! Please enter the name of your deploy module:'));
    if (!module_exists($module)) {
      return drush_user_abort("Module doesn't exist.");
    }
    else {
      variable_set('deploy_module', $module);
    }
  }
  if (!drush_confirm(dt('Do you want to add @extensions as a dependency to @deploy_module',
    array('@extensions' => $extensions, '@deploy_module' => variable_get('deploy_module'))))) {
    return drush_user_abort('Aborting.');
  }
  drush_append_dependency_write($extensions);
}

/**
 * Append the dependency string to the info file.
 */
function drush_append_dependency_write($extensions) {
  // @TODO: Create alter hook.
  $exceptions = array(variable_get('deploy_module'), 'devel', 'drush_append_dependency');
  if (in_array($extensions, $exceptions)) {
    return;
  }
  $path = drupal_get_path('module', variable_get('deploy_module'));
  $dependency = PHP_EOL . 'dependencies[] = ' . $extensions;
  $file = $path . '/' . variable_get('deploy_module') . '.info';
  if (strpos(file_get_contents($file), $dependency) === FALSE) {
    file_put_contents($file, $dependency, FILE_APPEND);
  }
}

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * Remove the module name from the info file when disabling a module.
 */
function drush_drush_append_dependency_pre_pm_disable($extensions) {

  $path = drupal_get_path('module', variable_get('deploy_module'));
  $dependency = 'dependencies[] = ' . $extensions;
  $file = $path . '/' . variable_get('deploy_module') . '.info';
  $contents = file_get_contents($file);
  $contents = str_replace($dependency, '', $contents);
  file_put_contents($file, $contents);

}

/**
 * Remove the dependency string from the deploy info file.
 */
function drush_append_dependency_remove($extensions) {
  $path = drupal_get_path('module', variable_get('deploy_module'));
  $dependency = "dependencies[] = " . $extensions;
  $file = $path . '/' . variable_get('deploy_module') . '.info';

  if (strpos(file_get_contents($file), $dependency) === TRUE) {
    file_put_contents($file, $dependency, FILE_APPEND);
  }
}
