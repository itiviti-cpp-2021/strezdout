(ns strezdout.bobthebuildr
  (:require [strezdout.osutil :refer :all]
            [clojure.tools.logging :as logging]
            [clojure.java.shell :refer [sh]]))

(defn build! "Create temp directory, copy sources to it and build" [task' config sources includes]
  (let [{{:keys [testee executable]} :tests} config
        task (name task')
        directory (canonize (tmpdir!) task)
        task-path (testers-canonize task)
        build-path (canonize directory "build")]
    (logging/info ::build "Preparing testee" {:task task :path directory :task-path task-path})
    (copy! task-path directory) ; copy prebuilt stuff first
    (copy! includes (canonize directory "include")) ; then copy the includes + sources
    (copy! sources (canonize directory "src"))
    (mkdir! build-path)
    (logging/info ::build "Building"
                  (cmake! build-path directory)
                  (make! build-path executable))
    (copy! (canonize build-path executable) (canonize directory testee))
    (rm! build-path) ; finally cleanup and return the directory
    directory))

(defn launch-options "Get launch options for launching a specified generator" [[type path]]
  (case type
    :clojure ["lein" "exec" path]
    :binary [path]))
