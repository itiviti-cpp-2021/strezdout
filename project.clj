(defproject strezdout "0.1.0-SNAPSHOT"
  :description "FIXME: write description"
  :url "http://example.com/FIXME"
  :license {:name "APACHE LICENSE, VERSION 2.0"
            :url "https://www.apache.org/licenses/LICENSE-2.0"}
  :dependencies [[org.clojure/clojure "1.10.1"]
                 [babashka/process "0.0.2"]]
  :main ^:skip-aot strezdout.core
  :target-path "target/%s"
  :profiles {})
