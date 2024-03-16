#[cfg(not(debug_assertions))]
use actix_files::Files;
use actix_web::{get, App, HttpResponse, HttpServer, Responder};
use rand::Rng;
use serde_json::json;

#[get("/api/temperature")]
async fn temperature() -> impl Responder {
    let temperature_cels = rand::thread_rng().gen_range(0..100);
    HttpResponse::Ok().json(json!({
        "temperature": temperature_cels
    }))
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    dotenvy::from_filename(".env.dev").expect("Failed to load .env.dev file");

    println!("ENV: {}", std::env::var("FRONTEND_STATIC_PATH").unwrap());
    HttpServer::new(move || {
        let app = App::new()
            .service(temperature);

        // Serve static files only in production mode
        #[cfg(not(debug_assertions))]
        return app.service(Files::new(
            "/", 
            std::env::var("FRONTEND_STATIC_PATH").expect("FRONTEND_STATIC_PATH environment variable wasn't found. Specify location of bundled frontend files using FRONTEND_STATIC_PATH environment variable.")
        ).index_file("index.html"));

        #[cfg(debug_assertions)]
        return app;
    })
    .bind(("127.0.0.1", 8080))?
    .run()
    .await
}
